//
//  Resolver.cpp
//  DNSResolver
//
//  Created by Aaron Hartman on 08 Nov 14.
//  Copyright (c) 2014 Aaron Hartman. All rights reserved.
//

#include "Resolver.h"

#include "DNSConstants.h"
#include "Message.h"
#include "UDPSocket.h"

#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;



///
/// Constructs a Resolver object
///
Resolver::Resolver(string domain, uint16_t recType, uint16_t recClass) : _domain(domain), _prevDomain(""), _address(""), _type(recType), _class(recClass), _authorityServer("")
{
	
	_rootServers.push_back(A_ROOT);
	_rootServers.push_back(B_ROOT);
	_rootServers.push_back(C_ROOT);
	_rootServers.push_back(D_ROOT);
	_rootServers.push_back(E_ROOT);
	_rootServers.push_back(F_ROOT);
	_rootServers.push_back(G_ROOT);
	_rootServers.push_back(H_ROOT);
	_rootServers.push_back(I_ROOT);
	_rootServers.push_back(J_ROOT);
	_rootServers.push_back(K_ROOT);
	_rootServers.push_back(L_ROOT);
	_rootServers.push_back(M_ROOT);

	_rootServerIter = _rootServers.begin();

	_authorityServer = *_rootServerIter;

    
    
    if(_type == DNSConstants::RRSIG_TYPE)
        _resolveRequest = new ResolveRequest(_domain, _type, _class, _authorityServer, true);
    else
        _resolveRequest = new ResolveRequest(_domain, _type, _class, _authorityServer, false);
    
    
}

///
/// Function to resolve the requested hostname.
/// @returns	const string containing the resolved address.
///
string Resolver::resolve()
{
    

	// Base case
	if(_resolveRequest->isResolved())
	{
		_address = _resolveRequest->getResolvedAddress();
		delete _resolveRequest;
        return _address;
	}

	/* 	string to contain IP of server to request from 	*/
    string server;

    cout << "Requesting \"" << _resolveRequest->getHostName() << "\" from [" << _resolveRequest->getRequestServer() << "] (Type: " << _resolveRequest->getRequestType() << ")" << endl;


    Message* request;

    // IF we need to request RRSIG record AND it is now the time to do it
    if(_resolveRequest->hasRRSIG() && _resolveRequest->getMakeRRSIGRequest())
    {
    	request = new Message(_resolveRequest->getHostName(), _resolveRequest->getRequestType(), _resolveRequest->getClassType());

    	// prevent recursion from final request
    	_resolveRequest->setRRSIG(false);
    }
    // ELSE we don't need to request the RRSIG record
    else
    {
        request = new Message(_resolveRequest->getHostName(), DNSConstants::A_TYPE, _resolveRequest->getClassType());
    }

    
	/**
	 * Send the Request
	 */
    
    Message* response = handleRequest(request);
    
    Header* responseHeader = response->getHeader();
    
	// IF there is an answer
	if(responseHeader->getNumAnswers() > 0)
	{
		// IF we need to get an RRSIG record
		if(_resolveRequest->hasRRSIG())
		{
			// We want to make the same previous request but requesting the RRSIG information
            _resolveRequest->setMakeRRSIGRequest(true);

            return resolve();
		}
		

		// IF there is a single answer 
		if(responseHeader->getNumAnswers() == 1)
		{

			ResourceRecord* answer = response->getAnswers()->at(0);

			// IF the type is the requested type (A/AAAA)
			if(answer->getType() == _resolveRequest->getRequestType())
            {
                _resolveRequest->setResolvedAddress(answer->getRData());

                return resolve();
            }
			// ELSEIF the single answer is a CNAME
			else if(answer->getType() == DNSConstants::CNAME_TYPE)
            {
                _resolveRequest->setHostName(answer->getRData());
                _resolveRequest->setRequestServer(*_rootServerIter);
                
                return resolve();
            }
				

		}// ENDIF
		// ELSEIF there is more than one answer
		else if(responseHeader->getNumAnswers() > 1)
		{
    
			// FOR every answer given
			for(vector<ResourceRecord *>::iterator iter = response->getAnswers()->begin(); iter != response->getAnswers()->end(); ++iter)
			{
				// IF the answer is of the requested type
				if((*iter)->getType() == _resolveRequest->getRequestType())
                {
                    _resolveRequest->setResolvedAddress((*iter)->getRData());
                    
                    return resolve();
                }

			} // ENDFOR ON ANSWERS GIVEN


            return "Domain exists, but no record for requested type.";
			

		}// ENDELSEIF ON MULTIPLE ANSWER

	} // ENDIF ON ANSWER
	// ELSEIF there is additonal records
	else if(responseHeader->getNumAdditionals() > 0)
	{
		// FOR every additional record
		for(vector<ResourceRecord *>::iterator iter = response->getAdditionals()->begin(); iter != response->getAdditionals()->end(); ++iter)
        {
        	// IF the type of the record is an A type
        	if((*iter)->getType() == DNSConstants::A_TYPE)
        	{

        		// Recursively resolve
        		_resolveRequest->setRequestServer((*iter)->getRData());

            	return resolve();
        	}
            
        }

	}// ENDELSEIF ON ADDITIONAL RECORDS
    // ELSEIF there is a Authority(NS) record
    else if(responseHeader->getNumNameServers() > 0)
    {
        // IF we want an RRSIG
        if(_resolveRequest->getRequestType() == DNSConstants::RRSIG_TYPE)
        {
            
        }
    }
	// ELSE there are no answers and no additional records
	else
	{
		// increment the iterator to the next root server
		_rootServerIter++;

		// IF the iterator is null (no more servers)
		if(_rootServerIter == _rootServers.end())
        {
            _resolveRequest->setResolvedAddress("Domain could not be found!");
            return resolve();
        }
		// ELSE there is another root server
		else
        {
            _resolveRequest->setRequestServer(*_rootServerIter);
            return resolve(); // Recursively resolve with new root server
        }

	}// ENDELSE ON NO ANSWERS/NO ADDITIONAL
    
    return "Domain does not exist!";
}


///
/// Function to perform making the request and handling any
/// error states related to.
///
///	@param request 	Message reference of the request to be made.
/// @returns Message pointer to the response->
///
Message* Resolver::handleRequest(Message* request)
{
    /* number of bytes the message is made up of        */
    unsigned int messageBytes = 0;

	/**
	 * Send the request
	 */
	UDPSocket socket(_resolveRequest->getRequestServer(), DNS_PORT);

	uint8_t* packetData = request->getMessageBytes(messageBytes);

	socket.write(packetData, messageBytes);



	/**
	 * Receive the response
	 */
    uint8_t* responseBytes = socket.read(messageBytes);

    Message* response = new Message(responseBytes);


    
    // IF the ID of our request and the ID of the response do not match
    if(request->getHeader()->getID() != response->getHeader()->getID())
    	throw runtime_error("DNS Response had ID mismatch!");		// this shouldn't ever happen


	// IF the response says this was NXDOMAIN
    if(response->getHeader()->getRCode() == DNSConstants::NX_RCODE)
    {
        _address = "NXDOMAIN";
        return NULL;
    }
    else
    {
        Header* headerTest = response->getHeader();
        headerTest->getRCode();
    	return response;
    }

}


///
/// Private method to extract the answer from a DNS Message
///
void Resolver::extractAnswer(Message* dnsAnswer)
{
	
}