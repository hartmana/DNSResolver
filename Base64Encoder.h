//
//  Base64Encoder.h
//  DNSResolver
//
//  Created by Aaron Hartman on 25 Nov 14.
//  Copyright (c) 2014 Aaron Hartman. All rights reserved.
//

#ifndef __DNSResolver__Base64Encoder__
#define __DNSResolver__Base64Encoder__


#include "Encoder.h"

#include <unordered_map>
#include <cstdint>


using std::unordered_map;

class Base64Encoder : public Encoder
{

private:

	/*  Map to contain our integer to char conversion   */
	unordered_map<uint8_t, char> _encodingMap;

	/* 	Map to contain our char to integer conversion 	*/
	unordered_map<char, uint8_t> _decodingMap;

public:

	Base64Encoder();

	virtual vector<uint8_t> *encode(uint8_t *data, size_t dataLength);

	virtual vector<uint8_t> *decode(uint8_t *data, size_t dataLength);

};

#endif /* defined(__DNSResolver__Base64Encoder__) */
