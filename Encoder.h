//
//  Encoder.h
//  DNSResolver
//
//  Created by Aaron Hartman on 25 Nov 14.
//  Copyright (c) 2014 Aaron Hartman. All rights reserved.
//

#ifndef __DNSResolver__Encoder__
#define __DNSResolver__Encoder__


#include <cstdint>
#include <vector>
#include <cstring>


using std::vector;


class Encoder
{

private:


public:

	Encoder() {};


	inline virtual vector<uint8_t> *encode(uint8_t* encodeData, size_t dataLength) { return NULL; }

	inline virtual vector<uint8_t> *decode(uint8_t* decodeData, size_t dataLength) { return NULL; }


};

#endif /* defined(__DNSResolver__Encoder__) */
