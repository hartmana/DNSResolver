//
//  EncodingFactory.h
//  DNSResolver
//
//  Created by Aaron Hartman on 25 Nov 14.
//  Copyright (c) 2014 Aaron Hartman. All rights reserved.
//

#ifndef __DNSResolver__EncodingFactory__
#define __DNSResolver__EncodingFactory__


#include "Encoder.h"

#include <string> 
#include <cstdint>


using std::string;


/**
* Types to be passed in to createEncoder by application programs
*/
const uint8_t BASE64 = 0;


///
/// Class to provide instantiations for different utility encoding classes
///
class EncodingFactory
{

private:

	static EncodingFactory *_instance;

protected:

	EncodingFactory()
	{
	};

public:

	static EncodingFactory *getInstance();

	Encoder *createEncoder(uint8_t encodingType);

};


#endif /* defined(__DNSResolver__EncodingFactory__) */
