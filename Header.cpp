//
//  Header.cpp
//  DNSResolver
//
//  Created by Aaron Hartman on 08 Nov 14.
//  Copyright (c) 2014 Aaron Hartman. All rights reserved.
//

#include "Header.h"

#include <iostream>
#include <stdexcept>
#include <cstring>


using std::runtime_error;


///
///	Default Constructor to build a complete Header
///
Header::Header()
{

    _ID = 0x0000;
	setNewID();

	_flags = 0x0000;
	_qdCount = 0x0000;
	_anCount = 0x0000;
	_nsCount = 0x0000;
	_arCount = 0x0000;

	// _headerBytes = 0;

}


///
/// Standard constructor to parse a byte stream to construct the Header.
/// Assumes that only the 12 bytes necessary for the header are passed.
///
Header::Header(uint8_t* &headerBytes)
{

	_ID = 0x0000;
	_flags = 0x0000;
	_qdCount = 0x0000;
	_anCount = 0x0000;
	_nsCount = 0x0000;
	_arCount = 0x0000;

	unsigned short index = 0;

	_ID |= (headerBytes[index++] << 8);
	_ID |= (headerBytes[index++]);

	_flags |= (headerBytes[index++] << 8);
	_flags |= headerBytes[index++];

	_qdCount |= (headerBytes[index++] << 8);
	_qdCount |= headerBytes[index++];

	_anCount |= (headerBytes[index++] << 8);
	_anCount |= headerBytes[index++];

	_nsCount |= (headerBytes[index++] << 8);
	_nsCount |= headerBytes[index++];

	_arCount |= (headerBytes[index++] << 8);
	_arCount |= headerBytes[index];

	// _headerBytes = 0;

}


///
/// Function to set the corresponding bits in the header denoting whether this is a query or response.
///
/// @param queryResponse 	bool value denoting if this is a query (false) or response (true).
///
void Header::setQueryResponse(const bool queryResponse)
{

	// IF query
	if (queryResponse)
	{
		_flags |= ((0x8000) & 0x8000);
	}
	else
	{
		_flags &= ((0x0000) & 0x8000);
	}
	
}


///
/// Function to get whether or not this is a query or response
///
/// @returns	bool value denoting if this is a query (false) or response (true).
///
const unsigned short Header::getQueryResponse()
{

	return static_cast<unsigned short>(_flags & 0x8000);
}


///
/// Function to set the corresponding bits in the header for the OPCODE. Note that
/// the mutator and accessor for the OPCODE only takes into account the 1-3 values
/// that are currently being used. 
///
/// @param opCode 	unsigned short containing the OPCODE of the overlying message.
///
void Header::setOpcode(const unsigned short opCode)
{

	_flags |= ((opCode << 12) & 0x1800);
}


///
/// Function to get the OPCODE for this header.
///
/// @returns	unsigned short containing the OPCODE of the overlying message.
///
const unsigned short Header::getOpcode()
{

	return static_cast<unsigned short>(_flags & 0x1800);
}


///
/// Function to set whether or not the overlying message is an authoritative server for the question.
///
/// @param isAA 	bool value denoting if this is an AA or not.
///
void Header::setAuthoritativeAnswer(const bool isAA)
{

	if (isAA)
	{
		_flags |= (0x0400);
	}
	else
	{
		_flags &= (0x0000 & 0x0400);
	}

}


///
/// Function to get whether or not the overlying message is an authoritative server for the question.
///
/// @returns	bool value denoting if this is an AA or not.
///
const unsigned short Header::getAuthoritativeAnswer()
{

	return static_cast<unsigned short>(_flags & 0x0400);
}


///
/// Function to set whether or not the message was truncated.
///
/// @param isTruncated 	bool value denoting if the overlying message was truncated.
///
void Header::setTruncated(const bool isTruncated)
{

	if (isTruncated)
	{
		_flags |= (0x0200);
	}
	else
	{
		_flags &= (0x0000 & 0x0200);
	}

}


///
/// Function to get whether or not the message was truncated.
///
/// @returns	bool value denoting if the overlying message was truncated.
///
const unsigned short Header::getTruncated()
{

	return static_cast<unsigned short>(_flags & 0x0200);
}


///
/// Function to set whether recursion is desired (for requests).
///
/// @param recursionDesired 	bool value denoting if recursion is desired.
///
void Header::setRecursionDesired(const bool recursionDesired)
{

	if (recursionDesired)
	{
		_flags |= (0x0100);
	}
	else
	{
		_flags &= (0x0000 & 0x0100);
	}
	
}


///
/// Function to get whether recursion is desired (for requests).
///
/// @returns	bool value denoting if recursion is desired.
///
const unsigned short Header::getRecursionDesired()
{

	return static_cast<unsigned short>(_flags & 0x0100);
}


///
/// Function to set whether or not recursion is available.
///
/// @param recursionAvailable 	bool value denoting if recursion is available.
///
void Header::setRecursionAvailable(const bool recursionAvailable)
{

	if (recursionAvailable)
	{
		_flags |= (0x0080);
	}
	else
	{
		_flags &= (0x0000 & 0x0080);
	}
	
}


///
/// Function to get whether or not recursion is available.
///
/// @returns	bool value denoting if recursion is available.
///
const unsigned short Header::getRecursionAvailable()
{

	return static_cast<unsigned short>(_flags & 0x0080);
}


///
/// Function to set the RCODE associated with the overlying message.
///
/// @param rCode 	unsigned short containing the RCODE specified in the DNS RFC (1035).
///
void Header::setRCode(const unsigned short rCode)
{

	if (rCode > 5)
	{
		throw runtime_error("RCODE for Header must be 0-5!");
	}
	
	_flags |= rCode;

}


///
/// Function to get the RCODE associated with the overlying message.
///
/// @returns 	unsigned short containing the RCODE specified in the DNS RFC (1035).
///
const unsigned short Header::getRCode()
{

	return static_cast<unsigned short>((_flags & 0x000F));
}


///
/// Function to assign a new random ID to this header.
///
void Header::setNewID()
{
	/**
	 * Set the ID to a random integer within the range of 16 bits
	 */
	srand(static_cast<unsigned int>(time(NULL)));

	_ID = static_cast<uint16_t>((rand() % 65535));
}



///
/// Function to build the unsigned char array representing this header and return it.
///
/// @returns 	uint8_t* to a 12 byte array containing all the fields of the Header.
///
uint8_t* Header::getHeaderBytes()
{

	// IF we haven't initialized the header bytes
	// if (_headerBytes == 0)
	// {
	// 	_headerBytes = new uint8_t[12];
	// }
	// else
	// {
	// 	// Instead of verifying the data is up to date, redoing it is faster
	// 	delete _headerBytes;
	// 	_headerBytes = 0;
	// 	_headerBytes = new uint8_t[12];
	// }

	memset(_headerBytes, 0x00, sizeof(_headerBytes));

	_headerBytes[0] |= (_ID >> 8);
	_headerBytes[1] |= _ID;
	_headerBytes[2] |= (_flags >> 8);
	_headerBytes[3] |= _flags;
	_headerBytes[4] |= (_qdCount >> 8);
	_headerBytes[5] |= _qdCount;
	_headerBytes[6] |= (_anCount >> 8);
	_headerBytes[7] |= _anCount;
	_headerBytes[8] |= (_nsCount >> 8);
	_headerBytes[9] |= _nsCount;
	_headerBytes[10] |= (_arCount >> 8);
	_headerBytes[11] |= _arCount;

	return _headerBytes;
}