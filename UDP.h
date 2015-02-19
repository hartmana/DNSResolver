//
//  UDP.h
//  Resolver
//
//  Created by Aaron Hartman on 05 Dec 14.
//  Copyright (c) 2014 Aaron Hartman. All rights reserved.
//

#ifndef UDP_H
#define UDP_H


#define MAX_BYTES 1024
#define NOT_CONNECTED -1000


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
/// Class to abstract the details away from using sockets.
///
class UDP {
    
private:
    
    int _socketDescriptor;
    struct addrinfo *_socketAddr;
    
    string _ipAddress;
    string _portNum;
    

    string _clientIpAddress;
    string _clientPortNum;
    
    uint8_t *_data;
    
    void initData(uint16_t size);
    void initPacket();
    void setClientInfo(struct sockaddr_storage& their_addr);
    void* get_in_addr(struct sockaddr *sa);
    void* get_in_port(struct sockaddr *sa);
    
public:
    
    // Destructor
    ~UDP();
    
    // Constructor to wrap a socket file descriptor with a UDP object
    UDP(int tcpSocketFD, struct sockaddr_storage& their_addr);
    
    // Constructor to setup a client socket to a given port and IP
    UDP(string ipAddr, string port);
    
    // Function to read from the connection and return the resulting data's address
    uint8_t* read(unsigned int& bytesRead);
    
    // Function to write to the connection
    bool write(uint8_t* &dataToSend, size_t dataToSendLength);
    
    
    inline int getSocketDescriptor() { return _socketDescriptor; }
    
    inline string getHostIpAddress() { return _ipAddress; }
    
    inline string getClientIpAddress() { return _clientIpAddress; }
    
    inline string getHostPort() { return _portNum; }
    
    inline string getClientPort() { return _clientPortNum; }
    
    inline uint8_t* getData() { return _data; }
    
    
};


#endif /* defined(__Resolver__UDP__) */
