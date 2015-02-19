//
//  Header.h
//  DNSResolver
//
//  Created by Aaron Hartman on 08 Nov 14.
//  Copyright (c) 2014 Aaron Hartman. All rights reserved.
//

#ifndef HEADER_H
#define HEADER_H


#include <iostream>
#include <stdexcept>
#include <cstring>


using std::runtime_error;


///
/// Class representing a DNS Message Header
///
class Header
{

private:

	uint16_t _ID;        	// Random ID to be set by the constructor for any new Header created
	uint16_t _flags;		// 16 bit flags field to contain QR (1), OPCODE (4), AA (1), TC (1), RD (1), RA (1), Z (3), RCODE(4)
	uint16_t _qdCount;    	// Question desired count
	uint16_t _anCount;    	// Answers received count
	uint16_t _nsCount;    	// Name Server received count
	uint16_t _arCount;    	// Additional records received count

	// uint8_t *_headerBytes;
	uint8_t _headerBytes[12];


public:

	// Destructor
	~Header()
	{

		// if (_headerBytes != 0) delete [] _headerBytes;
	}


	// Default constructor
	Header();

	// Standard constructor to parse a byte stream to construct the Header
	Header(uint8_t* &headerBytes);

	// Sets whether this is a query (false) or response (true)
	void setQueryResponse(const bool queryResponse);

	const unsigned short getQueryResponse();

	// Sets the OPCODE for the header
	void setOpcode(const unsigned short opCode);

	const unsigned short getOpcode();

	// Sets whether or not the message this header belongs to is an authoritative answer
	void setAuthoritativeAnswer(const bool isAA);

	const unsigned short getAuthoritativeAnswer();

	// Sets whether the message was truncated
	void setTruncated(const bool isTruncated);

	const unsigned short getTruncated();

	// Sets whether recursion is desired or not
	void setRecursionDesired(const bool recursionDesired);

	const unsigned short getRecursionDesired();

	// Sets whether recursion is available
	void setRecursionAvailable(const bool queryResponse);

	const unsigned short getRecursionAvailable();
	
	/* 	Note Z is defaulted to 0 	*/

	// Sets the RCODE for this header
	void setRCode(const unsigned short rCode);

	const unsigned short getRCode();


	// Sets how many question records are in the containing message
	inline void setNumQuestions(const unsigned short numQuestions)	{ _qdCount = numQuestions; }
	inline const unsigned short getNumQuestions() { return _qdCount; }

	// Sets how many answer records are in the containing message
	inline void setNumAnswers(const unsigned short numAnswers)	{ _anCount = numAnswers; }
	inline const unsigned short getNumAnswers()	{ return _anCount; }

	// Sets how many name servers records are in the containing message
	inline void setNumNameServers(const unsigned short numNameServers) { _nsCount = numNameServers; }
	inline unsigned short getNumNameServers() {	return _nsCount; }

	// Sets how many additional records are in the containing message
	inline void setNumAdditionals(const unsigned short numAdditionals) { _arCount = numAdditionals;	}
	inline unsigned short getNumAdditionals() {	return _arCount; }

	void setNewID();
	inline unsigned short getID() {	return _ID;	}

	uint8_t* getHeaderBytes();

};

#endif
