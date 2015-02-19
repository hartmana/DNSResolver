//
//  main.cpp
//  DNSResolver
//
//  Created by Aaron Hartman on 08 Nov 14.
//  Copyright (c) 2014 Aaron Hartman. All rights reserved.
//

#include "UDPSocket.h"
#include "Resolver.h"
#include "Message.h"


#include <iostream>
#include <unistd.h>
#include <string>


using std::cout;
using std::endl;
using std::string;


///
/// Print the correct usage in case of user syntax error.
/// DNSResolver takes a mandatory domain name to be found
/// as well as an optional type.
///
void usage(char *arg0)
{

	cout << "Usage: " << arg0 << " <URL> [RRTYPE (A/AAAA)]" << endl;
	cout << "URL is mandatory and should be specified as www.google.com." << endl;
	cout << "RRTYPE is optional and should either be A or AAAA." << endl;

	exit(EXIT_FAILURE);
}


int main(int argc, char *const argv[])
{

	cout << "argc: " << argc << endl;

	// IF user didn't enter enough options
	if (argc == 1 || argc > 3)
	{
		usage(*argv);
	}

	string domain(argv[1]);
	string rType("A");

	if (argc == 3)
	{
		rType = argv[2];
	}

	cout << "domain: " << domain << endl;

    uint16_t type = 0x0000;
    
    if(rType == "A")
        type = DNSConstants::A_TYPE;
    else if(rType == "AAAA")
        type = DNSConstants::AAAA_TYPE;
    else if(rType == "RRSIG")
    	type = DNSConstants::RRSIG_TYPE;
    else
    {
        cout << "FUCKED" << endl;
        return EXIT_FAILURE;
    }

	try
	{
		 Resolver resolver(domain, type, DNSConstants::IN_CLASS);

		 cout << "Resolved Address: " << resolver.resolve() << endl;

//        Message message(domain, DNSConstants::A_TYPE, DNSConstants::IN_CLASS);
//
//        UDPSocket socket("198.41.0.4", "53");
//
//
//        unsigned int messageBytes = 0;
//        uint8_t* messData = message.getMessageBytes(messageBytes);
//
//        socket.write(messData, messageBytes);
//
//        uint8_t* response = socket.read(messageBytes);
//
//        Message responseMessage(response);
//
//        cout << message.getHeader()->getID() << endl;
//        cout << responseMessage.getHeader()->getID() << endl;
        

	}
	catch (std::runtime_error &e)
	{
		cout << "Error resolving domain: " << e.what() << endl;
	}


	return 0;
}
