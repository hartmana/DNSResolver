//
//  Question.cpp
//  DNSResolver
//
//  Created by Aaron Hartman on 08 Nov 14.
//  Copyright (c) 2014 Aaron Hartman. All rights reserved.
//

#include "Question.h"

#include "DNSCompression.h"


///
/// Standard constructor to parse a byte stream to construct the question
///
/// @param questionBytes 	unsigned char array containing the question
/// @param startOctet 		unsigned int reference with the current position in the byte stream.
/// @param compressor 		DNSCompression reference to be used for compression/decompression of domain names.
///
Question::Question(uint8_t* &questionBytes, unsigned int &startOctet, DNSCompression &compressor)
{

	_compressor = compressor;

	_qName = _compressor.decompress(questionBytes, startOctet);

	_qType = 0x0000;
	_qClass = 0x0000;

	// decode type
	_qType |= (questionBytes[startOctet++] << 8);
	_qType |= questionBytes[startOctet++];

	// decode class
	_qClass |= (questionBytes[startOctet++] << 8);
	_qClass |= questionBytes[startOctet++];
    
    _questionBytes = 0;

}


///
/// Function to build the unsigned char array representing this question and return it.
///
/// @param startOctet 	unsigned int reference to the starting octet position where the record will be placed.
/// 					(used for compression)
/// @param returnedBytes	unsigned int reference to be modified with the number of bytes returned.
///
/// @returns 	uint8_t* to a char array containing all the fields of the Question.
///
uint8_t* Question::getQuestionBytes(unsigned int &startOctet, unsigned int &returnedSize)
{

	if (_questionBytes != 0)
	{
		delete _questionBytes;
        _questionBytes = 0;
	}

	vector<uint8_t> compressedDomain = _compressor.compress(_qName, startOctet);


	/* 	Initialize new array with the size of compressed domain + class and type 	*/
	returnedSize = static_cast<unsigned int>(compressedDomain.size() + 4);
	_questionBytes = new uint8_t[returnedSize];
	memset(_questionBytes, 0x00, (sizeof(uint8_t) * returnedSize));

	unsigned int index = 0;

	while(index < compressedDomain.size())
	{
		_questionBytes[index] = compressedDomain[index];
		++index;
	}

	_questionBytes[index++] |= (_qType >> 8);
	_questionBytes[index++] |= _qType;

	_questionBytes[index++] |= (_qClass >> 8);
	_questionBytes[index] |= _qClass;


	return _questionBytes;
}