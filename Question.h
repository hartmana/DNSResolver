//
//  Question.h
//  DNSResolver
//
//  Created by Aaron Hartman on 08 Nov 14.
//  Copyright (c) 2014 Aaron Hartman. All rights reserved.
//

#ifndef QUESTION_H
#define QUESTION_H


#include "DNSCompression.h"

#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <string>


using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::stringstream;
using std::runtime_error;


///
/// Class representing a DNS Question
///
class Question
{

private:

	string _qName;        	// Domain name in question
	uint16_t _qType;		// Type of the question
	uint16_t _qClass;    	// Class of the question

	uint8_t *_questionBytes;

	DNSCompression _compressor;


public:

	~Question() { if (_questionBytes != 0) delete _questionBytes; }

	// Default constructor
	Question(DNSCompression &compressor) : _qName(""), _qType(1), _qClass(1), _questionBytes(0), _compressor(compressor) {};    // Defaults type to A records and class to IN

	// Standard constructor to parse a byte stream to construct the Question
	Question(uint8_t* &questionBytes, unsigned int &startOctet, DNSCompression &compressor);


	inline void setQName(const string qName) { _qName = qName; }
	inline const string getQName() { return _qName;	}

	inline void setQType(const unsigned short qType) { _qType = qType; }
	inline const unsigned short getQType() { return _qType;	}

	inline void setClassType(const unsigned short cType) { _qClass = cType;	}
	inline const unsigned short getClassType() { return _qType;	}

	// startOctet is used for compression where the current position in the overall encoded stream is needed
	uint8_t *getQuestionBytes(unsigned int &startOctet, unsigned int &returnedSize);

};

#endif
