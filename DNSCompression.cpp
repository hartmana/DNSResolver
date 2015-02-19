//
//  DNSCompression.cpp
//  DNSResolver
//
//  Created by Aaron Hartman on 08 Nov 14.
//  Copyright (c) 2014 Aaron Hartman. All rights reserved.
//

#include "DNSCompression.h"
#include "StringUtils.h"

#include <string>
#include <cstdint>
#include <unordered_map>
#include <vector>


using std::string;
using std::unordered_map;
using std::vector;


///
///	Function to compress a given string according to the DNS Packet
/// compression in RFC 1035 4.1.4.
///
/// @param data 	string containing the data to be compressed.
///
/// @param startOctet 	Reference to an unsigned int denoting the starting octet of where
///						the compressed string was in the DNS Packet. Note that this value WILL be modified
///						with the last octet that was encoded. ie, startOctet + size of returned
///						array.
///
/// @return 	a pointer to a vector of uint8_t allocated on the heap containing the compressed string.
///
vector<uint8_t> DNSCompression::compress(string data, unsigned int &startOctet)
{


	/*	vector for the compressed data 		*/
	vector<uint8_t> compressedData;

	uint8_t currOctet = 0;
	uint8_t currOctetLength = 0;
	uint16_t octetPointer = 0xC000;   // first two bits denote that the octet is a pointer

	/**
	* Before anything is done, if this domain has already been encoded we can simply return a pointer
	*/

	// Get an iterator to the current domain in the map
	unordered_map<string, uint16_t>::const_iterator mapKey = _compressionMap.find(data);

	// IF the map contains this key
	if (mapKey != _compressionMap.end())
	{
		/**
		* Retrieve the octet position of the prior occurence of the current piece of the domain and
		* encode that position as a pointer instead of re-encoding the entire piece.
		*/
		octetPointer = 0x0000;
		octetPointer = (mapKey->second);   // get the pointer

		currOctet = 0x00;       // reset the current octet
		currOctet |=
				((octetPointer >> 8) & 0x00FF);            // store the leftmost 8 bits of the pointer in the current octet
		compressedData.push_back(currOctet);
		++startOctet;

		currOctet = 0x00;
		currOctet |=
				(octetPointer & 0x00FF);                   // store the rightmost 8 bits of the pointer in the current octet
		compressedData.push_back(currOctet);
		++startOctet;
	}
	// ELSE the domain has not been encoded before
	else
	{

		/**
		* Add the domain and its starting location to the map
		*/
		octetPointer = 0xC000;      // reset pointer
		octetPointer |= static_cast<uint16_t>(startOctet);
		_compressionMap.insert(std::make_pair(data, octetPointer));    // add it to the set


		/**
		* Encode the domain by subdomain with each subdomain being preceded
		* by the length in octets of that subdomain.
		*/
		StringUtils stringUtils;
		vector<string> domainSplits = stringUtils.split(data, '.');


		// FOR every piece of the domain
		for (vector<string>::iterator iter = domainSplits.begin(); iter != domainSplits.end(); ++iter)
		{
			/**
			* Encode the length followed by the octets of the current domain piece, while also adding
			* this domain component and its position to our map.
			*/
			currOctetLength = 0x00;
			currOctetLength = iter->size();
			compressedData.push_back(currOctetLength);
			++startOctet;


			// FOR every character in the current domain component
			for (string::const_iterator dcIter = iter->begin(); dcIter != iter->end(); ++dcIter)
			{
				currOctet = 0x00;
				currOctet |= (*dcIter & 0xFF);
				compressedData.push_back(currOctet);
				++startOctet;
			}

		}
	}


	/**
	* Encode a 0 so the decoder knows where the end of the domain is.
	*/
	currOctet = 0x00;
	compressedData.push_back(currOctet);
	++startOctet;

	return compressedData;         // return a pointer to the underlying data


}


///
/// Function to decompress a given string according to the DNS Packet
/// compression in RFC 1035 4.1.4.
/// Note:   This function will work to decompress non-compressed fields as well
///         ie., the signer's name on a RRSIG RR.
///
/// @param encodedData  string containing the data to be decompressed.
///
/// @param startOctet   Reference to an unsigned int denoting the starting octet of
///                     the compressed string. Note that this value WILL be modified
///                     with the last octet that was decoded. ie, startOctet + number of
///                     decoded octets.
///
/// @return     a decoded string.
///
string DNSCompression::decompress(uint8_t* &encodedData, unsigned int &startOctet)
{

	/*  string to contain the decompressed domain       */
	string domain;


	uint8_t currOctet = 0x00;
	uint8_t currOctetLength = 0x00;
	uint16_t octetPointer = 0x0000;


	// get the first octet
	currOctet = encodedData[startOctet];



	// WHILE we haven't reached a 0
	while (currOctet != 0x00)
	{

		// IF the first two bits of the current octet are a 1
		if (currOctet & 0xC0)
		{

			/**
			* We have a pointer, so we need the remaining bits in the current
			* octet plus the next octet for reference.
			*/
			octetPointer = 0x0000;
			octetPointer |= ((currOctet << 8) & 0x00111111);
			octetPointer |= encodedData[++startOctet];  // get the next octet
			++startOctet;   // advance to the position after the pointer

			unsigned int compressedPos = static_cast<unsigned int>(octetPointer);
			string pointedDomain = decompress(encodedData, compressedPos);


			// IF the domain we have been working with so far already contains something
			if (domain.size() > 0)
			{
				// we know the pointed to domain needs to be appended to what already exists
				domain.append(pointedDomain);

				return domain;
			}
				// ELSE the entire domain is referenced by the pointer
			else
			{
				return pointedDomain;
			}


		}
			// ELSE we expect a length octet
		else
		{

			currOctetLength = currOctet;    // get the total length, in octets, of following characters


			/*  string containing only the portion of the encoded data that we want     */
			string domainPart
					(reinterpret_cast<char*>(encodedData + ++startOctet), static_cast<unsigned int>(currOctetLength));

			domain.append(domainPart);
			domain.append(".");

			startOctet +=
					static_cast<unsigned int>(currOctetLength);   // update the index to position after what we extracted

		}

		// get the octet for comparison in the while loop
		currOctet = encodedData[startOctet];

	}

	++startOctet;   // increment our position past our finishing null octet


	if (domain.size() > 0)
	{

		// Extricate the trailing '.'
		string::iterator lastPos = (domain.end() - 1);
		domain.erase(lastPos);

	}

	// return the string minus the trailing '.'
	return domain;

}

