//
//  Resolver.h
//  DNSResolver
//
//  Created by Aaron Hartman on 08 Nov 14.
//  Copyright (c) 2014 Aaron Hartman. All rights reserved.
//

#ifndef RESOLVER_H
#define RESOLVER_H


#include "DNSConstants.h"
#include "Message.h"
#include "UDPSocket.h"

#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;


class Resolver
{

private:

	/// 
	/// INNER Class representing a resolve request to be passed to the resolve method
	///
	class ResolveRequest {

	private:
		string _hostname;
		uint16_t _requestType;
    	uint16_t _classType;
    	string _requestServer;
    	string _cname;
    	bool _RRSIG;
    	bool _makeRRSIGRequest;
    
		string _resolvedAddress;
	

	public:

        ResolveRequest(string domain, uint16_t requestType, uint16_t classType, string requestServer, bool RRSIG) : _hostname(domain), _requestType(requestType), _classType(classType), _requestServer(requestServer), _cname(""), _RRSIG(RRSIG), _makeRRSIGRequest(false), _resolvedAddress("") {};

		inline void setHostName(string hostname) { _hostname = hostname; }
		inline const string getHostName() { return _hostname; }
		
		inline void setRequestType(uint16_t requestType) { _requestType = requestType; }
		inline const uint16_t getRequestType() { return _requestType; }

		inline void setClassType(uint16_t classType) { _classType = classType; }
		inline const uint16_t getClassType() { return _classType; }

		inline void setRequestServer(string requestServer) { _requestServer = requestServer; }
		inline const string getRequestServer() { return _requestServer; }

		inline void setCName(string cname) { _cname = cname; }
		inline string getCName() { return _cname; }
		inline bool hasCName() { return _cname.size() > 0; }
		
		inline bool isResolved() { return _resolvedAddress != ""; }

		inline void setResolvedAddress(string resolvedAddress) { _resolvedAddress = resolvedAddress; }
		inline const string getResolvedAddress() { return _resolvedAddress; }

		inline void setRRSIG(bool RRSIG) { _RRSIG = RRSIG; }
		inline const bool hasRRSIG() { return _RRSIG; }

		inline void setMakeRRSIGRequest(bool makeRRSIGRequest) { _makeRRSIGRequest = makeRRSIGRequest; }
		inline const bool getMakeRRSIGRequest() { return _makeRRSIGRequest; }

	};


	string _domain;				// Domain to resolve 	
	string _prevDomain;			// Used when actually resolving 
	string _address;			// Used for resolved address	

	uint16_t _type;				// type of record to request
	uint16_t _class;			// class of record to request

	string _authorityServer;	// string containing address of the server to make the request from

	vector<string> _rootServers;
	vector<string>::const_iterator _rootServerIter;

	ResolveRequest* _resolveRequest;


	Message* handleRequest(Message* request);
	void extractAnswer(Message* dnsAnswer);


public:

	Resolver(string domain, uint16_t recType, uint16_t recClass);

	string resolve();

	inline const bool isResolved() { return (_address.size() > 0);}
	inline const string getResolvedAddress() { return _address; };
};


#endif