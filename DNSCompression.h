//
//  DNSCompression.h
//  DNSResolver
//
//  Created by Aaron Hartman on 08 Nov 14.
//  Copyright (c) 2014 Aaron Hartman. All rights reserved.
//

#ifndef DNSCOMPRESSIONUTILS_H
#define DNSCOMPRESSIONUTILS_H


#include "StringUtils.h"

#include <string>
#include <cstdint>
#include <unordered_map>
#include <vector>


using std::string;
using std::unordered_map;
using std::vector;


class DNSCompression
{

private:

	unordered_map<string, uint16_t> _compressionMap;

public:

	DNSCompression() {};

	vector<uint8_t> compress(string data, unsigned int &startOctet);

	string decompress(uint8_t* &encodedData, unsigned int &startOctet);

};

#endif
