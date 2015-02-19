//
//  Message.h
//  DNSResolver
//
//  Created by Aaron Hartman on 08 Nov 14.
//  Copyright (c) 2014 Aaron Hartman. All rights reserved.
//

#ifndef MESSAGE_H
#define MESSAGE_H


#include "Header.h"
#include "Question.h"
#include "ResourceRecord.h"
#include "DNSCompression.h"

#include <iostream>
#include <vector>
#include <string>


using std::string;
using std::runtime_error;
using std::vector;


///
/// Class representing a DNS Message
///
class Message
{

private:

	Header *_header;
	vector<Question> _questions;
	vector<ResourceRecord> _records;

	DNSCompression* _compressor;

	uint8_t* _messageBytes;


public:

	~Message()
	{

		if (_header != 0) delete _header;
		if(_compressor != 0) delete _compressor;
		if (_messageBytes != 0) delete _messageBytes;

	}


	// Default constructor
	Message(string domain, uint16_t cl, uint16_t type);

	// Standard constructor to parse a byte stream to construct the Message
	Message(uint8_t* &messageBytes);


	inline Header* getHeader() { return _header; }

	vector<Question*>* getQuestions();
	vector<ResourceRecord*>* getAnswers();
	vector<ResourceRecord*>* getNameServers();
	vector<ResourceRecord*>* getAdditionals();

	uint8_t* getMessageBytes(unsigned int& numMessageBytes);

	
};

#endif
