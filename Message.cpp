//
//  Message.cpp
//  DNSResolver
//
//  Created by Aaron Hartman on 08 Nov 14.
//  Copyright (c) 2014 Aaron Hartman. All rights reserved.
//

#include "Message.h"


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
/// Constructor to build a DNS request message from the given domain, class, and type.
///
Message::Message(string domain, uint16_t cl, uint16_t type)
{

	_header = new Header();

	_header->setNumQuestions(1);

	_compressor = new DNSCompression;

	_messageBytes = 0;

	Question question(*_compressor);

	question.setQName(domain);
	question.setClassType(cl);
	question.setQType(type);

	_questions.push_back(question);


}


/// 
/// Constructor to build a Message from a byte stream
///
Message::Message(uint8_t* &messageBytes)
{

	_header = new Header(messageBytes);
	_compressor = new DNSCompression;

	_messageBytes = 0;

	// Header occupies 12 bytes, so we start there
	unsigned int currOctet = 12;


	// FOR every question the header specified
	for (unsigned short numQ = 0; numQ < _header->getNumQuestions(); ++numQ)
	{
		_questions.push_back(Question(messageBytes, currOctet, *_compressor));
	}

	// FOR every answer the header specified
	for (unsigned short numA = 0; numA < _header->getNumAnswers(); ++numA)
	{
		_records.push_back(ResourceRecord(messageBytes, currOctet, *_compressor, DNSConstants::ANSWER_RECORD));
	}

	// FOR every name server the header specified
	for (unsigned short numNS = 0; numNS < _header->getNumNameServers(); ++numNS)
	{
		_records.push_back(ResourceRecord(messageBytes, currOctet, *_compressor, DNSConstants::NS_RECORD));
	}

	// FOR every additional the header specified
	for (unsigned short numAd = 0; numAd < _header->getNumAdditionals(); ++numAd)
	{
		_records.push_back(ResourceRecord(messageBytes, currOctet, *_compressor, DNSConstants::ADDITIONAL_RECORD));
	}
}


///
/// Function to get the question records pertaining to this DNS message.
/// @returns	vector pointer to vector containing Question record pointers.
///
vector<Question*>* Message::getQuestions()
{

	vector<Question*>* questions = new vector<Question*>;

	for (size_t i = 0; i < _questions.size(); ++i)
	{
		questions->push_back(&_questions[i]);
	}

	return questions;
}


///
/// Function to get the answer records pertaining to this DNS message.
/// @returns	vector pointer to vector containing Answer record pointers.
///
vector<ResourceRecord*>* Message::getAnswers()
{

	vector<ResourceRecord*>* answers = new vector<ResourceRecord*>;

	for (size_t i = 0; i < _records.size(); ++i)
	{
		if (_records[i].getRecordType() == DNSConstants::ANSWER_RECORD)
		{
			answers->push_back(&_records[i]);
		}
	}

	return answers;
}


///
/// Function to get the name server records pertaining to this DNS message.
/// @returns	vector pointer to vector containing Name Server record pointers.
///
vector<ResourceRecord*>* Message::getNameServers()
{

	vector<ResourceRecord*>* nameServers = new vector<ResourceRecord*>;

	for (size_t i = 0; i < _records.size(); ++i)
	{
		if (_records[i].getRecordType() == DNSConstants::NS_RECORD)
		{
			nameServers->push_back(&_records[i]);
		}
	}

	return nameServers;
}


///
/// Function to get the additional records pertaining to this DNS message.
/// @returns	vector pointer to vector containing Additional record pointers.
///
vector<ResourceRecord*>* Message::getAdditionals()
{

	vector<ResourceRecord*>* additionals = new vector<ResourceRecord*>;

	for (size_t i = 0; i < _records.size(); ++i)
	{
		if (_records[i].getRecordType() == DNSConstants::ADDITIONAL_RECORD)
		{
			additionals->push_back(&_records[i]);
		}
	}

	return additionals;
}


///
/// Function to get message in byte array form.
/// @param 		numMessageBytes 	unsigned int reference that will be modified with the length in bytes of the returned array.
/// @returns	const unsigned char pointer.
///
uint8_t* Message::getMessageBytes(unsigned int& numMessageBytes)
{

	uint8_t* headerBytes = _header->getHeaderBytes();

	unsigned int currOctet = 0;
	unsigned int totalSize = 0;

	vector<uint8_t>* messageBytes = new vector<uint8_t>();
	// vector<uint8_t> messageBytes;


	// FOR every byte of the header
	for(int index = 0; index < 12; ++index)
		messageBytes->push_back(headerBytes[currOctet++]);


	// FOR every question needing to be added
	for(size_t numQ = 0; numQ < _questions.size(); ++numQ)
	{

		uint8_t* questionBytes = _questions[numQ].getQuestionBytes(currOctet, totalSize);

		for(unsigned int i = 0; i < totalSize; ++i)
		{
			messageBytes->push_back(questionBytes[i]);
		}

	}


	numMessageBytes = static_cast<unsigned int>(messageBytes->size());

	return messageBytes->data();

}
