//
//  EncodingFactory.cpp
//  DNSResolver
//
//  Created by Aaron Hartman on 25 Nov 14.
//  Copyright (c) 2014 Aaron Hartman. All rights reserved.
//

#include "EncodingFactory.h"
#include "Base64Encoder.h"

#include <iostream> 


using std::runtime_error;

EncodingFactory *EncodingFactory::_instance = 0;


EncodingFactory *EncodingFactory::getInstance()
{

	if (_instance == 0)
	{
		_instance = new EncodingFactory();
	}

	return _instance;
}


Encoder *EncodingFactory::createEncoder(uint8_t encodingType)
{

	switch (encodingType)
	{
		case BASE64:

			return new Base64Encoder();
			break;

		default:
			throw runtime_error("Encoder type not supported in EncodingFactory!\n");
			break;

	}
}