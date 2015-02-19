//
//  ResourceRecord.h
//  DNSResolver
//
//  Created by Aaron Hartman on 08 Nov 14.
//  Copyright (c) 2014 Aaron Hartman. All rights reserved.
//

#ifndef RESOURCERECORD_H
#define RESOURCERECORD_H


#include "DNSCompression.h"
#include "DNSConstants.h"
#include "StringUtils.h"

#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>


using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::stringstream;
using std::runtime_error;


///
/// Class representing a DNS Resource Record
///
class ResourceRecord
{

private:

	string _domain;       	// string containing the domain this record pertains to
	uint16_t _type;        	// type of this resource record (A/AAAA/etc)
	uint16_t _class;    	// class of this record (IN/CHAOS/etc)
	uint32_t _ttl;        	// time to live of the record
	uint16_t _rdLength; 	// length in bytes of the rData field
	string _rData;        	// variable length field containing the data according to the resource record type and class

	vector<uint8_t> *_rSig;

	DNSCompression _compressor;

	unsigned short _recordType;


	string extractAddress(uint8_t* &encodedData, unsigned int &startOctet);

	string extractIPV6Address(uint8_t* &encodedData, unsigned int &startOctet);

public:

	// Default constructor
	ResourceRecord(string domain, uint16_t type, uint16_t cl, uint16_t ttl, DNSCompression &compressor, const unsigned short recordType)
			: _domain(domain),
			  _type(type),
			  _class(cl),
			  _ttl(ttl),
			  _rdLength(0),
			  _rData(0),
			  _compressor(compressor),
			  _recordType(recordType)
	{
	};

	ResourceRecord(uint8_t* &recordBytes, unsigned int &startOctet, DNSCompression &compressor, const unsigned short recordType);


	inline void setDomain(const string domain) { _domain = domain; }
	inline const string getDomain() { return _domain; }

	inline void setType(const unsigned short type) { _type = type; }
	inline const unsigned short getType() { return _type; }

	inline void setClass(const unsigned short cType) { _class = cType; }
	inline const unsigned short getClass() { return _class; }

	inline void setTTL(const unsigned int ttl) { _ttl = ttl; }
	inline const unsigned int getTTL() { return _ttl; }

	inline const unsigned short getRDLength() {	return _rdLength; }

	inline const string getRData() { return _rData; }

	inline const vector<uint8_t> *getRSig()	{ return _rSig; }

	inline const unsigned short getRecordType() { return _recordType; }

};

#endif
