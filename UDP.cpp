//
//  UDP
//  DNSResolver
//
//  Created by Aaron Hartman on 08 Nov 14.
//  Copyright (c) 2014 Aaron Hartman. All rights reserved.
//

#include "UDP.h"


#include <iostream>
#include <vector>
#include <stdexcept>
#include <cstring>
#include <cstdint>
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sstream>


using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::stringstream;
using std::runtime_error;
using std::vector;


///
/// Function name: socket constructor
/// Description: initializes a TCPSocket object around a TCP socket file descriptor.
/// Parameters: int denoting the file descriptor of the connection.
///             struct suckaddr_storage reference
/// Returns: TCPSocket object
///
UDP::UDP(int udpSocketFD, struct sockaddr_storage& their_addr)
{
    
    
    /**
     * Initialize the defaults for a server connection
     */
    _socketDescriptor = udpSocketFD;
    _socketAddr = NULL;
    _ipAddress = "";
    _portNum = "0";
    _clientIpAddress = "";
    _clientPortNum = "";
    _data = NULL;
    
    
    /**
     * Set the client and host IP and port number of this TCPSocket
     */
//    setClientInfo(their_addr);
//    setHostInfo();
    
}

///
/// Function name: client Socket constructor
/// Description: initializes a server socket to listen for connections
/// Parameters: n/a
/// Returns: UDPSocket client object
///
UDP::UDP(string ipAddr, string port)
{
    
    /**
     * Initialize the defaults for a server connection
     */
    _socketDescriptor = NOT_CONNECTED;
    _socketAddr = NULL;
    _ipAddress = "";
    _portNum = port;
    _clientIpAddress = ipAddr;
    _clientPortNum = port;
    _data = NULL;
    
    
    // IF setting up the client fails
//    if(!setupClient())
//        throw runtime_error("Failure setting up client!");
    
}

///
/// Function name: Destructor
/// Description: Reclaims allocated memory from the Socket data and packet data pointers,
///              where applicable.
///
UDP::~UDP()
{
    if(_data != NULL)
    {
        delete [] _data;
        _data = NULL;
    }
    
    // if(_socketAddr != NULL)
    // {
    //     delete _socketAddr;
    //     _socketAddr = NULL;
    // }
    
    
    shutdown(_socketDescriptor, SHUT_WR);
}


///
/// Function name: initData
/// Description: Helper function to initialize the _data member to the specified max data
///              size. If it has already been initialized it will be deleted beforehand.
/// Parameters: n/a
/// Returns: void
///
void UDP::initData(uint16_t size)
{
    
    // IF we have already allocated space for a read before
    if(_data != NULL)
    {
        delete [] _data;
        _data = NULL;
    }
    
    _data = new unsigned char[size];
    
}


///
/// Function name: setClientInfo
/// Description: Helper function to set the IP address and Port number of the client computer this
///              socket is connected to.
/// Parameters: n/a
/// Returns: void
///
void UDP::setClientInfo(struct sockaddr_storage& their_addr)
{
    /**
     * Verify the passed in file descriptor and get the client information
     */
    
    char clientAddress[INET6_ADDRSTRLEN];
    memset(&clientAddress, '\0', INET6_ADDRSTRLEN);
    
    
    // IF there was a failure accepting
    if (_socketDescriptor == -1)
    {
        throw runtime_error("UDP: Given an invalid socket descriptor.");
    }
    else
    {
        // Convert the connecting clients address to presentation form
        inet_ntop(their_addr.ss_family,
                  get_in_addr((struct sockaddr *) &their_addr),
                  clientAddress, sizeof clientAddress);
        
        
        // set this objects client IP address accordingly
        _clientIpAddress = clientAddress;
        
        /**
         * Store the port the server is listening on
         */
        
        struct sockaddr_in sin;
        socklen_t addrlen = sizeof(sin);
        
        if (getsockname(_socketDescriptor, (struct sockaddr *) &sin, &addrlen) == 0 &&
            sin.sin_family == AF_INET &&
            addrlen == sizeof(sin)) {
            stringstream ss;
            ss << ntohs(sin.sin_port);
            ss >> _clientPortNum;
        }
    }
}

///
/// Function name: setHostInfo
/// Description: Helper function to set the IP address and Port number of the host computer
///              this client UDP is running on.
/// Parameters: n/a
/// Returns: void
///
//void UDP::setHostInfo()
//{
//    char actualAddress[INET6_ADDRSTRLEN];
//    memset(&actualAddress, '\0', INET6_ADDRSTRLEN);
//    
//    gethostname(actualAddress, INET6_ADDRSTRLEN);
//    
//    cout << "Actual Address: " << actualAddress << endl;
//    cout << "sizeof: " << sizeof(actualAddress) << endl;
//    
//    
//    struct hostent* hostAddrInfo = gethostbyname(actualAddress);
//    
//    
//    if(hostAddrInfo == NULL)
//    {
//        cout << actualAddress << " is unavailable" << endl;
//    }
//    
//    in_addr * address = (in_addr * )hostAddrInfo->h_addr;
//    
//    
//    // set this Socket objects ip address accordingly
//    _ipAddress = inet_ntoa(* address);
//    
//    
//    
//    /**
//     * Store the port this socket is at
//     */
//    
//    struct sockaddr_in sin;
//    socklen_t addrlen = sizeof(sin);
//    
//    if(getsockname(_socketDescriptor, (struct sockaddr *)&sin, &addrlen) == 0 &&
//       sin.sin_family == AF_INET &&
//       addrlen == sizeof(sin))
//    {
//        stringstream ss;
//        ss << ntohs(sin.sin_port);
//        ss >> _portNum;
//    }
//    
//    
//    
//    if(hostAddrInfo != NULL)
//        delete hostAddrInfo;
//    if(address != NULL)
//        delete address;
//    
//}


///
/// Function name: setupClient
/// Description: initalizes a server socket to listenForCon for connections
/// Parameters: n/a
/// Returns: bool denoting its success or failure
///
//bool UDP::setupClient()
//{
//    
//    struct addrinfo hints;
//    struct addrinfo *servinfo;
//    int rv;
//    
//    
//    memset(&hints, 0, sizeof hints);
//    
//    hints.ai_family = AF_UNSPEC;
//    hints.ai_socktype = SOCK_DGRAM;
//    
//    
//    // IF filling the addrinfo struct fails
//    if ((rv = getaddrinfo(_clientIpAddress.c_str(), _clientPortNum.c_str(), &hints, &servinfo)) != 0) {
//        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
//        
//        cout << "Address: " << _clientIpAddress << ":" << _clientPortNum << endl;
//        return false;
//    }
//    
//    // loop through all the results and connect to the first we can
//    for(_socketAddr = servinfo; _socketAddr != NULL; _socketAddr = _socketAddr->ai_next)
//    {
//        // IF initializing the socket fails
//        if ((_socketDescriptor = socket(_socketAddr->ai_family, _socketAddr->ai_socktype,
//                                        _socketAddr->ai_protocol)) == -1)
//        {
//            
//            perror("setupClient() socket call failed.");
//            
//            continue;
//            
//        }
//        
//        // IF connecting to the server fails
//        if (connect(_socketDescriptor, _socketAddr->ai_addr, _socketAddr->ai_addrlen) == -1)
//        {
//            
//            close(_socketDescriptor);
//            
//            perror("setupClient() connect call failed.");
//            
//            continue;
//            
//        }
//        
//        break;
//    }
//    
//    
//    // IF we didn't make any connection
//    if (_socketAddr == NULL) {
//        fprintf(stderr, "setupClient(): Failed to connect to %s.\n", _clientIpAddress.c_str());
//        return false;
//    }
//    
//    
//    freeaddrinfo(servinfo); // all done with this structure
//    
//    // Set the host info for later use
//    // setHostInfo();
//    
//    return true;    // successfully setup socket
//    
//}


///
/// read function to read a stream of bytes from the established connection.
///
/// @param bytesRead    Reference to an unsigned int where the total bytes read will be stored.
/// @return             Address to a uint8_t array containing binary data obtained from the read.
///                     [Note: Memory managed my class]
///
uint8_t* UDP::read(unsigned int& bytesRead)
{
    
    int sockfd = 0;
    struct addrinfo hints, *servinfo, *p;
    int rv = 0;
    int numbytes = 0;
    struct sockaddr_storage their_addr;
    socklen_t addr_len;
    char s[INET6_ADDRSTRLEN];

    initData(MAX_BYTES);

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(_ipAddress.c_str(), _portNum.c_str(), &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("listener: socket");
            continue;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("listener: bind");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "listener: failed to bind socket\n");
    }

    freeaddrinfo(servinfo);

    printf("listener: waiting to recvfrom...\n");

    addr_len = sizeof their_addr;
    if ((numbytes = recvfrom(sockfd, _data, MAX_BYTES - 1 , 0,
        (struct sockaddr *)&their_addr, &addr_len)) == -1) {
        perror("recvfrom");
        exit(1);
    }

    printf("listener: got packet from %s\n",
        inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof s));
    
    _data[MAX_BYTES] = '\0';

    shutdown(sockfd, SHUT_WR);

    bytesRead = numbytes;
    
    return  _data;
    
}


///
/// write function to write a stream of bytes with a designated length to the established connection.
///
/// @param dataToSend       pointer to a uint8_t array to be written to the socket.
/// @param dataToSendLength number of bytes dataToSend pointer represents.
/// @return                 bool denoting its success or failure
///
bool UDP::write(uint8_t* &dataToSend, size_t dataToSendLength)
{
    
    int sockfd = 0;
    struct addrinfo hints, *servinfo, *p;
    int rv = 0;
    int numbytes = 0;


    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ((rv = getaddrinfo(_ipAddress.c_str(), _portNum.c_str(), &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and make a socket
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("talker: socket");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "talker: failed to bind socket\n");
        return 2;
    }

    if ((numbytes = sendto(sockfd, dataToSend, dataToSendLength, 0,
             p->ai_addr, p->ai_addrlen)) == -1) {
        perror("talker: sendto");
        exit(1);
    }

    freeaddrinfo(servinfo);

    printf("talker: sent %d bytes to %s\n", numbytes, _ipAddress.c_str());
shutdown(sockfd, SHUT_WR);

    
    
    
    // verify socket is connected
//    checkConnSocket();
    
    
    /************************************************************************
     * Change this so that the length of the data is encoded into an array and
     * sent first from within write(). A separate send will be used for the
     * actual data.
     ************************************************************************/
    
    
    
//    ssize_t bytesSent = 0;
//    size_t totalBytesSent = 0;
//    
//    
//    while(totalBytesSent < dataToSendLength)
//    {
//        
//        
//        bytesSent = send(_socketDescriptor, static_cast<void *>(dataToSend + totalBytesSent), (dataToSendLength - totalBytesSent), 0);
//        
//        // IF sending the message fails
//        if(bytesSent == -1)
//        {
//            perror("Error writing data in write().");
//            return false;
//        }
//        else
//        {
//            totalBytesSent += bytesSent;
//        }
//        
//    }
    
//    cout << "wrote " << totalBytesSent << " bytes" << endl;
    
    return true;
    
}


///
/// Function name: checkConnSocket
/// Description: Verifies that the connected socket is indeed connected.
/// Parameters: n/a
/// Returns: bool denoting its success or failure
///
//void UDP::checkConnSocket()
//{
//    // IF our bound socket is not set
//    if(_socketDescriptor == NOT_CONNECTED)
//        throw runtime_error("Error with socket. _currConnSocketDesc is not connected!");
//    
//}

///
/// Function to return the address in a sockaddr structure,
/// regardless of IPv4 or IPv6.
///
void*UDP::get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

///
/// Function to return the port in a sockaddr structure,
/// regardless of IPv4 or IPv6.
///
void*UDP::get_in_port(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_port);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_port);
}
