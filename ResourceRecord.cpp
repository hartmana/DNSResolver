//
//  ResourceRecord.cpp
//  DNSResolver
//
//  Created by Aaron Hartman on 08 Nov 14.
//  Copyright (c) 2014 Aaron Hartman. All rights reserved.
//

#include "ResourceRecord.h"

#include "DNSCompression.h"
#include "DNSConstants.h"
#include "StringUtils.h"
#include "EncodingFactory.h"

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
/// Constructor to construct the resource record out of a byte stream and the record location
/// in that byte stream.
///
/// @param recordBytes 	unsigned char pointer reference pointing to the data containing the record.
/// @param startOctet 	unsigned int reference with the starting location in the byte stream of the record
/// 					to be extracted. Note that it WILL be modified with the first position after the record.
///
ResourceRecord::ResourceRecord(uint8_t* &recordBytes, unsigned int &startOctet, DNSCompression &compressor, const unsigned short recordType)
{

	_type = 0x0000;
	_class = 0x0000;
	_ttl = 0x00000000;
	_rdLength = 0x0000;
	_rData = "";

	_compressor = compressor;

	_rSig = 0;

	_recordType = recordType;

	// decode the domain name
	_domain = _compressor.decompress(recordBytes, startOctet);

	// decode _type
	_type |= (recordBytes[startOctet++] << 8);
	_type |= recordBytes[startOctet++];

	// decode class
	_class |= (recordBytes[startOctet++] << 8);
	_class |= recordBytes[startOctet++];

	// decode the ttl
	_ttl |= (recordBytes[startOctet++] << 24);
	_ttl |= (recordBytes[startOctet++] << 16);
	_ttl |= (recordBytes[startOctet++] << 8);
	_ttl |= recordBytes[startOctet++];


    

	switch(_type)
	{
		case DNSConstants::A_TYPE:
        {
			// skip 2 octets containing the length of the following address
			startOctet += 2;

			_rData = extractAddress(recordBytes, startOctet);      // extract ipv4

			break;
        }
		case DNSConstants::AAAA_TYPE:
        {
			// skip 2 octets containing the length of the following address
			startOctet += 2;

			_rData = extractIPV6Address(recordBytes, startOctet);

			break;
        }
		case DNSConstants::CNAME_TYPE:
        {
			// skip 2 octets containing the length of the following CNAME
			startOctet += 2;

			_rData = _compressor.decompress(recordBytes, startOctet);          // decompress the CNAME

			break;
        }
		case DNSConstants::NS_TYPE:
        {
			// skip 2 octets containing the length of the following NS name
			startOctet += 2;

			_rData = _compressor.decompress(recordBytes, startOctet);          // decompress the NS name

			break;
        }
		case DNSConstants::RRSIG_TYPE:
        {
            


			uint16_t dataLength = 0x0000;
			uint16_t _typeCovered = 0x0000;
			uint8_t algorithm = 0x00;
			uint8_t labelCount = 0x00;
			uint32_t origTTL = 0x00000000;
			uint32_t sigExp = 0x00000000;
			uint32_t sigInc = 0x00000000;
			uint16_t keyTag = 0x0000;
			string signerName;
			string signature;

			dataLength |= (recordBytes[startOctet++] << 8);
			dataLength |= recordBytes[startOctet++];



			/* Get the position after the data length field for later use.
			 * We know that the remaining data in the RRSIG answer is dataLength
			 * bytes, but because the signerName field is variable, we have to know
			 * the number of bytes read between here and the end of the signerName
			 * to accurately extract the right number of bytes for the signature    */
			unsigned int beginOctet = startOctet;


			_typeCovered |= (recordBytes[startOctet++] << 8);
			_typeCovered |= recordBytes[startOctet++];

			algorithm |= recordBytes[startOctet++];

			labelCount |= recordBytes[startOctet++];

			origTTL |= (recordBytes[startOctet++] << 24);
			origTTL |= (recordBytes[startOctet++] << 16);
			origTTL |= (recordBytes[startOctet++] << 8);
			origTTL |= recordBytes[startOctet++];

			sigExp |= (recordBytes[startOctet++] << 24);
			sigExp |= (recordBytes[startOctet++] << 16);
			sigExp |= (recordBytes[startOctet++] << 8);
			sigExp |= recordBytes[startOctet++];

			sigInc |= (recordBytes[startOctet++] << 24);
			sigInc |= (recordBytes[startOctet++] << 16);
			sigInc |= (recordBytes[startOctet++] << 8);
			sigInc |= recordBytes[startOctet++];

			keyTag |= (recordBytes[startOctet++] << 8);
			keyTag |= recordBytes[startOctet++];


			// Signer name is not compressed, but function works independently of compression
			signerName = _compressor.decompress(recordBytes, startOctet);


			/*  calculated number of bytes of the actual signature      */
			unsigned int numSigBytes = (static_cast<unsigned int>(dataLength) - (startOctet - beginOctet));

			/*  Get the appropriate encoder         */
			EncodingFactory *encodingFactory = EncodingFactory::getInstance();
			Encoder *base64Encoder = encodingFactory->createEncoder(BASE64);



			// Encode the hex values to base 64
			_rSig = base64Encoder->encode(static_cast<uint8_t *>(recordBytes + startOctet), numSigBytes);

			startOctet += numSigBytes;

			break;
        }
        case DNSConstants::SOA_TYPE:
        {
        				uint16_t dataLength = 0x0000;
			uint16_t _typeCovered = 0x0000;
			uint8_t algorithm = 0x00;
			uint8_t labelCount = 0x00;
			uint32_t origTTL = 0x00000000;
			uint32_t sigExp = 0x00000000;
			uint32_t sigInc = 0x00000000;
			uint16_t keyTag = 0x0000;
			string signerName;
			string signature;

			dataLength |= (recordBytes[startOctet++] << 8);
			dataLength |= recordBytes[startOctet++];



			/* Get the position after the data length field for later use.
			 * We know that the remaining data in the RRSIG answer is dataLength
			 * bytes, but because the signerName field is variable, we have to know
			 * the number of bytes read between here and the end of the signerName
			 * to accurately extract the right number of bytes for the signature    */
			unsigned int beginOctet = startOctet;


			_typeCovered |= (recordBytes[startOctet++] << 8);
			_typeCovered |= recordBytes[startOctet++];

			algorithm |= recordBytes[startOctet++];

			labelCount |= recordBytes[startOctet++];

			origTTL |= (recordBytes[startOctet++] << 24);
			origTTL |= (recordBytes[startOctet++] << 16);
			origTTL |= (recordBytes[startOctet++] << 8);
			origTTL |= recordBytes[startOctet++];

			sigExp |= (recordBytes[startOctet++] << 24);
			sigExp |= (recordBytes[startOctet++] << 16);
			sigExp |= (recordBytes[startOctet++] << 8);
			sigExp |= recordBytes[startOctet++];

			sigInc |= (recordBytes[startOctet++] << 24);
			sigInc |= (recordBytes[startOctet++] << 16);
			sigInc |= (recordBytes[startOctet++] << 8);
			sigInc |= recordBytes[startOctet++];

			keyTag |= (recordBytes[startOctet++] << 8);
			keyTag |= recordBytes[startOctet++];


			// Signer name is not compressed, but function works independently of compression
			signerName = _compressor.decompress(recordBytes, startOctet);


			/*  calculated number of bytes of the actual signature      */
			unsigned int numSigBytes = (static_cast<unsigned int>(dataLength) - (startOctet - beginOctet));

			/*  Get the appropriate encoder         */
			EncodingFactory *encodingFactory = EncodingFactory::getInstance();
			Encoder *base64Encoder = encodingFactory->createEncoder(BASE64);



			// Encode the hex values to base 64
			_rSig = base64Encoder->encode(static_cast<uint8_t *>(recordBytes + startOctet), numSigBytes);

			startOctet += numSigBytes;

        	break;
        }
		default:
        {
			char error[50];

			sprintf(error, "_type [%d] not supported for answer record!", _type);

			throw runtime_error(error);

			break;
        }

	} // END SWITCH

}


///
/// Function to build the unsigned char array representing this record and return it.
///
/// @param startOctet 	unsigned int reference to the starting octet position where the record will be placed.
/// 					(used for compression)
/// @param returnedBytes	unsigned int reference to be modified with the number of bytes returned.
///
/// @returns 	uint8_t* to a char array containing all the fields of the Question.
///
// const uint8_t* ResourceRecord::getRecordBytes(unsigned int& startOctet, unsigned int& returnedSize)
// {
// 	if(_recordBytes != 0)
// 		delete _recordBytes;

// 	vector<uint8_t>* compressedDomain = _compressor.compress(_qName, startOctet);


// 	/* 	Initialize new array with the size of compressed domain + class and type 	*/
// 	_recordBytes = new uint8_t[compressedDomain->size() + 4];
// 	memset(_recordBytes, 0x00, sizeof(_recordBytes));

// 	unsigned int index = 0;

// 	for(; index < compressedDomain->size(); ++index)
// 		_recordBytes[index] = (*compressedDomain)[index];

// 	_recordBytes[index++] |= (_qType >> 8);
// 	_recordBytes[index++] |= _qType;

// 	_recordBytes[index++] |= (_qClass >> 8);
// 	_recordBytes[index] |= _qClass;


// 	returnedSize = sizeof(_recordBytes);

// 	return _recordBytes;
// }

///
/// Function to extract an IPV4 Address from a DNS packet.
///
/// @param encodedData  uint8_t array containing the data to be extracted.
///
/// @param startOctet   Reference to an unsigned int denoting the starting octet of 
///                     the address. Note that this value WILL be modified
///                     with the last octet that was decoded. ie, startOctet + number of 
///                     decoded octets.
///
/// @return     a decoded string.
///
string ResourceRecord::extractAddress(uint8_t* &encodedData, unsigned int &startOctet)
{

	stringstream ipStream;
	string currOctet;
	string ip;


	ipStream << static_cast<unsigned int>(encodedData[startOctet++]);
	ipStream >> currOctet;

	ip.append(currOctet);
	ip.append(".");

	ipStream.clear();
	ipStream << static_cast<unsigned int>(encodedData[startOctet++]);
	ipStream >> currOctet;

	ip.append(currOctet);
	ip.append(".");

	ipStream.clear();
	ipStream << static_cast<unsigned int>(encodedData[startOctet++]);
	ipStream >> currOctet;

	ip.append(currOctet);
	ip.append(".");

	ipStream.clear();
	ipStream << static_cast<unsigned int>(encodedData[startOctet++]);
	ipStream >> currOctet;

	ip.append(currOctet);


	return ip;

}


///
/// Function to extract an IPV6 Address from a DNS packet.
///
/// @param encodedData  uint8_t array containing the data to be extracted.
///
/// @param startOctet   Reference to an unsigned int denoting the starting octet of 
///                     the address. Note that this value WILL be modified
///                     with the last octet that was decoded. ie, startOctet + number of 
///                     decoded octets.
///
/// @return     a decoded string.
///
string ResourceRecord::extractIPV6Address(uint8_t* &encodedData, unsigned int &startOctet)
{

	/*  char array to hold each octet of our address    */
	char ipv6Addr[17][2];
	memset(ipv6Addr, '\0', sizeof(ipv6Addr[0][0]) * 17 * 2);

	/*  string to hold the assembled IPV6 address       */
	string ipv6Complete = "";


	// FOR every byte of the IPV6 address
	for (unsigned int i = 0; i < 16; ++i)
	{
		sprintf(ipv6Addr[i], "%02x", encodedData[startOctet++]);
	}    // get its character representation


	/*  char array to hold the assembled 16 bit field of the address   */
	char part[3];


	// FOR every field of the IPV6 address
	for (unsigned int j = 0; j < 16; ++j)
	{

		/**
		* Assemble it into a whole string that can be concatenated
		* on to our finished address.
		*/
		part[0] = ipv6Addr[j][0];
		part[1] = ipv6Addr[j][1];
		part[2] = '\0';

		ipv6Complete.append(part);

		if (j % 2 > 0)
		{
			ipv6Complete.append(":");
		}
	}



	/**
	* Split the newly assembled domain
	*/
	StringUtils sUtils;
	vector<string> splits = sUtils.split(ipv6Complete, ':');


	ipv6Complete = "";

	// FOR each split of the address
	for (vector<string>::iterator iter = splits.begin(); iter != splits.end(); ++iter)
	{
		// IF the current "hextet" of the address are zeroes  [IPV6 ZERO COMPRESSION]
		if (*iter == "0000")
		{
			// Copy the iterator at the current position for look-ahead
			vector<string>::iterator advancedIter = iter;

			// WHILE the following splits continue to be all zeroes
			while (advancedIter != splits.end() && *advancedIter == "0000") ++advancedIter;

			// IF the advanced iterator was advanced 2 or more
			if (advancedIter > (iter + 1))
			{
				ipv6Complete.append(":");   // truncate out 0'd "hextets"

				iter = (advancedIter - 1);        // catch up to the advanced position
			}
				// ELSE its only a single hextet of zeroes
			else
			{
				ipv6Complete.append(*iter);
			}

			continue;   // go to next hextet
		}
			// ELSE the hextet is not all zeroes, but we still want to check for leading zeroes  [IPV6 LEADING ZERO COMPRESSION]
		else
		{
			string::iterator leadZeroIter = (*iter).begin();

			// WHILE the individual niblets of the address continue to be 0's
			// NOTE: Because of the IF statement, there's no need to check for the end
			while (*leadZeroIter == '0')
			{
				(*iter).erase(leadZeroIter);
			}


			// append remaining parts of the hextet
			ipv6Complete.append(*iter);
			ipv6Complete.append(":");

			continue;   // go to next

		}

	}


	// Extricate the trailing ':'
	string::iterator lastPos = (ipv6Complete.end() - 1);
	ipv6Complete.erase(lastPos);


	return ipv6Complete;

}
