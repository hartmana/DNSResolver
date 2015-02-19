//
//  Base64Encoder.cpp
//  DNSResolver
//
//  Created by Aaron Hartman on 25 Nov 14.
//  Copyright (c) 2014 Aaron Hartman. All rights reserved.
//

#include "Base64Encoder.h"

#include <iostream>
#include <unordered_map>
#include <cstdint>
#include <vector>


using std::unordered_map;
using std::cout;
using std::endl;
using std::vector;


///
/// Constructor to initialize the translation map
///
Base64Encoder::Base64Encoder() : Encoder()
{

	_encodingMap.insert(std::make_pair(0, 'A'));
	_encodingMap.insert(std::make_pair(1, 'B'));
	_encodingMap.insert(std::make_pair(2, 'C'));
	_encodingMap.insert(std::make_pair(3, 'D'));
	_encodingMap.insert(std::make_pair(4, 'E'));
	_encodingMap.insert(std::make_pair(5, 'F'));
	_encodingMap.insert(std::make_pair(6, 'G'));
	_encodingMap.insert(std::make_pair(7, 'H'));
	_encodingMap.insert(std::make_pair(8, 'I'));
	_encodingMap.insert(std::make_pair(9, 'J'));
	_encodingMap.insert(std::make_pair(10, 'K'));
	_encodingMap.insert(std::make_pair(11, 'L'));
	_encodingMap.insert(std::make_pair(12, 'M'));
	_encodingMap.insert(std::make_pair(13, 'N'));
	_encodingMap.insert(std::make_pair(14, 'O'));
	_encodingMap.insert(std::make_pair(15, 'P'));
	_encodingMap.insert(std::make_pair(16, 'Q'));
	_encodingMap.insert(std::make_pair(17, 'R'));
	_encodingMap.insert(std::make_pair(18, 'S'));
	_encodingMap.insert(std::make_pair(19, 'T'));
	_encodingMap.insert(std::make_pair(20, 'U'));
	_encodingMap.insert(std::make_pair(21, 'V'));
	_encodingMap.insert(std::make_pair(22, 'W'));
	_encodingMap.insert(std::make_pair(23, 'X'));
	_encodingMap.insert(std::make_pair(24, 'Y'));
	_encodingMap.insert(std::make_pair(25, 'Z'));
	_encodingMap.insert(std::make_pair(26, 'a'));
	_encodingMap.insert(std::make_pair(27, 'b'));
	_encodingMap.insert(std::make_pair(28, 'c'));
	_encodingMap.insert(std::make_pair(29, 'd'));
	_encodingMap.insert(std::make_pair(30, 'e'));
	_encodingMap.insert(std::make_pair(31, 'f'));
	_encodingMap.insert(std::make_pair(32, 'g'));
	_encodingMap.insert(std::make_pair(33, 'h'));
	_encodingMap.insert(std::make_pair(34, 'i'));
	_encodingMap.insert(std::make_pair(35, 'j'));
	_encodingMap.insert(std::make_pair(36, 'k'));
	_encodingMap.insert(std::make_pair(37, 'l'));
	_encodingMap.insert(std::make_pair(38, 'm'));
	_encodingMap.insert(std::make_pair(39, 'n'));
	_encodingMap.insert(std::make_pair(40, 'o'));
	_encodingMap.insert(std::make_pair(41, 'p'));
	_encodingMap.insert(std::make_pair(42, 'q'));
	_encodingMap.insert(std::make_pair(43, 'r'));
	_encodingMap.insert(std::make_pair(44, 's'));
	_encodingMap.insert(std::make_pair(45, 't'));
	_encodingMap.insert(std::make_pair(46, 'u'));
	_encodingMap.insert(std::make_pair(47, 'v'));
	_encodingMap.insert(std::make_pair(48, 'w'));
	_encodingMap.insert(std::make_pair(49, 'x'));
	_encodingMap.insert(std::make_pair(50, 'y'));
	_encodingMap.insert(std::make_pair(51, 'z'));
	_encodingMap.insert(std::make_pair(52, '0'));
	_encodingMap.insert(std::make_pair(53, '1'));
	_encodingMap.insert(std::make_pair(54, '2'));
	_encodingMap.insert(std::make_pair(55, '3'));
	_encodingMap.insert(std::make_pair(56, '4'));
	_encodingMap.insert(std::make_pair(57, '5'));
	_encodingMap.insert(std::make_pair(58, '6'));
	_encodingMap.insert(std::make_pair(59, '7'));
	_encodingMap.insert(std::make_pair(60, '8'));
	_encodingMap.insert(std::make_pair(61, '9'));
	_encodingMap.insert(std::make_pair(62, '+'));
	_encodingMap.insert(std::make_pair(63, '/'));


	_decodingMap.insert(std::make_pair('A', 0));
	_decodingMap.insert(std::make_pair('B', 1));
	_decodingMap.insert(std::make_pair('C', 2));
	_decodingMap.insert(std::make_pair('D', 3));
	_decodingMap.insert(std::make_pair('E', 4));
	_decodingMap.insert(std::make_pair('F', 5));
	_decodingMap.insert(std::make_pair('G', 6));
	_decodingMap.insert(std::make_pair('H', 7));
	_decodingMap.insert(std::make_pair('I', 8));
	_decodingMap.insert(std::make_pair('J', 9));
	_decodingMap.insert(std::make_pair('K', 10));
	_decodingMap.insert(std::make_pair('L', 11));
	_decodingMap.insert(std::make_pair('M', 12));
	_decodingMap.insert(std::make_pair('N', 13));
	_decodingMap.insert(std::make_pair('O', 14));
	_decodingMap.insert(std::make_pair('P', 15));
	_decodingMap.insert(std::make_pair('Q', 16));
	_decodingMap.insert(std::make_pair('R', 17));
	_decodingMap.insert(std::make_pair('S', 18));
	_decodingMap.insert(std::make_pair('T', 19));
	_decodingMap.insert(std::make_pair('U', 20));
	_decodingMap.insert(std::make_pair('V', 21));
	_decodingMap.insert(std::make_pair('W', 22));
	_decodingMap.insert(std::make_pair('X', 23));
	_decodingMap.insert(std::make_pair('Y', 24));
	_decodingMap.insert(std::make_pair('Z', 25));
	_decodingMap.insert(std::make_pair('a', 26));
	_decodingMap.insert(std::make_pair('b', 27));
	_decodingMap.insert(std::make_pair('c', 28));
	_decodingMap.insert(std::make_pair('d', 29));
	_decodingMap.insert(std::make_pair('e', 30));
	_decodingMap.insert(std::make_pair('f', 31));
	_decodingMap.insert(std::make_pair('g', 32));
	_decodingMap.insert(std::make_pair('h', 33));
	_decodingMap.insert(std::make_pair('i', 34));
	_decodingMap.insert(std::make_pair('j', 35));
	_decodingMap.insert(std::make_pair('k', 36));
	_decodingMap.insert(std::make_pair('l', 37));
	_decodingMap.insert(std::make_pair('m', 38));
	_decodingMap.insert(std::make_pair('n', 39));
	_decodingMap.insert(std::make_pair('o', 40));
	_decodingMap.insert(std::make_pair('p', 41));
	_decodingMap.insert(std::make_pair('q', 42));
	_decodingMap.insert(std::make_pair('r', 43));
	_decodingMap.insert(std::make_pair('s', 44));
	_decodingMap.insert(std::make_pair('t', 45));
	_decodingMap.insert(std::make_pair('u', 46));
	_decodingMap.insert(std::make_pair('v', 47));
	_decodingMap.insert(std::make_pair('w', 48));
	_decodingMap.insert(std::make_pair('x', 49));
	_decodingMap.insert(std::make_pair('y', 50));
	_decodingMap.insert(std::make_pair('z', 51));
	_decodingMap.insert(std::make_pair('0', 52));
	_decodingMap.insert(std::make_pair('1', 53));
	_decodingMap.insert(std::make_pair('2', 54));
	_decodingMap.insert(std::make_pair('3', 55));
	_decodingMap.insert(std::make_pair('4', 56));
	_decodingMap.insert(std::make_pair('5', 57));
	_decodingMap.insert(std::make_pair('6', 58));
	_decodingMap.insert(std::make_pair('7', 59));
	_decodingMap.insert(std::make_pair('8', 60));
	_decodingMap.insert(std::make_pair('9', 61));
	_decodingMap.insert(std::make_pair('+', 62));
	_decodingMap.insert(std::make_pair('/', 63));

}


///
/// Function to encode a char array to its Base64 equivalent according to
/// RFC 4648.
///
/// @param data         Char array of data to be encoded.
///
/// @param dataLength   Length of the char array to be encoded.
///
/// @return     pointer to vector of char's containing the Base64 equivalent of what was passed in.
///
vector<uint8_t> *Base64Encoder::encode(uint8_t *data, size_t dataLength)
{

	/* 	char array to contain the 4 individual 6 bit chunks from each 24 bit segment 	*/
	uint8_t sixBitChunks[4];

	/* 	vector to contain the encoded string 	*/
	vector<uint8_t> *encodedData = new vector<uint8_t>;

	size_t currPos = 0;
	unsigned int i = 0;

	// WHILE we still have 3 contiguous bytes left
	while (currPos <= (dataLength - 3))
	{

		/**
		* The whole idea of Base64 encoding is taking a 24 bit stream (or 3 octets) and
		* encoding them into 4 distinct 6 bit values. The numbers denoted by the 6 bits
		* are transposed with the character values found in RFC 4648.
		*/

		sixBitChunks[0] = 0x00;
		sixBitChunks[1] = 0x00;
		sixBitChunks[2] = 0x00;
		sixBitChunks[3] = 0x00;



		// Get the first 6 bits from the first byte
		sixBitChunks[0] |= ((data[currPos] >> 2) & 0x3F);

		// Get the remaining 2 bits from the first byte
		sixBitChunks[1] |= ((data[currPos++] << 4) & 0x30);

		// Get the next 4 bits from the second byte
		sixBitChunks[1] |= ((data[currPos] >> 4) & 0x0F);

		// Get the remaining 4 bits from second byte
		sixBitChunks[2] |= ((data[currPos++] << 2) & 0x3C);

		// Store the next 2 bits from the third byte
		sixBitChunks[2] |= ((data[currPos] >> 6) & 0x03);

		// Store the last 6 bits from the third byte
		sixBitChunks[3] |= (data[currPos++] & 0x3F);



		// FOR each of the 6 bit chunks
		for (i = 0; i <= 3; ++i)
		{

			// Get an iterator to the equivalent value in the map
			unordered_map<uint8_t, char>::const_iterator mapKey = _encodingMap.find(sixBitChunks[i]);

			// IF the map contains this key
			if (mapKey != _encodingMap.end())
			{
				encodedData->push_back(mapKey->second);    // store its encoding
			}
			else
			{
				cout << "Value not in map! (Base64Encoder::encode()" << endl;
			}

		}
		
	}


	/* 	Number of bytes that are remaining to be encoded 	*/
	size_t remainingBytes = dataLength - currPos;


	// IF we have 2 bytes left
	if (remainingBytes == 2)
	{

		sixBitChunks[0] = 0x00;
		sixBitChunks[1] = 0x00;
		sixBitChunks[2] = 0x00;


		sixBitChunks[0] |= ((data[currPos] >> 2) & 0x3F);
		sixBitChunks[1] |= ((data[currPos++] << 4) & 0x30);
		sixBitChunks[1] |= ((data[currPos] >> 4) & 0x0F);
		sixBitChunks[2] |= ((data[currPos++] << 2) & 0x3C);


		for (i = 0; i <= 2; ++i)
		{

			// Get an iterator to the equivalent value in the map
			unordered_map<uint8_t, char>::const_iterator mapKey = _encodingMap.find(sixBitChunks[i]);

			// IF the map contains this key
			if (mapKey != _encodingMap.end())
			{
				encodedData->push_back(mapKey->second);    // store its encoding
			}
			else
			{
				cout << "Value not in map! (Base64Encoder::encode()" << endl;
			}

		}

		// add the padding character
		encodedData->push_back('=');
		
	}
	else if (remainingBytes == 1)
	{

		sixBitChunks[0] = 0x00;
		sixBitChunks[1] = 0x00;


		sixBitChunks[0] |= ((data[currPos] >> 2) & 0x3F);
		sixBitChunks[1] |= ((data[currPos++] << 4) & 0x30);


		for (i = 0; i <= 1; ++i)
		{

			// Get an iterator to the equivalent value in the map
			unordered_map<uint8_t, char>::const_iterator mapKey = _encodingMap.find(sixBitChunks[i]);

			// IF the map contains this key
			if (mapKey != _encodingMap.end())
			{
				encodedData->push_back(mapKey->second);    // store its encoding
			}
			else
			{
				cout << "Value not in map! (Base64Encoder::encode()" << endl;
			}

		}


		// Add two padding chars
		encodedData->push_back('=');
		encodedData->push_back('=');

	}


	return encodedData;

}


///
/// Function to decode a char array from Base64 to standard char representation 
/// according to RFC 4648.
///
/// @param data         Char array of data to be decoded.
///
/// @param dataLength   Length of the char array to be decoded.
///
/// @return     pointer to vector of char's containing the decoded equivalent of what was passed in.
///
vector<uint8_t> *Base64Encoder::decode(uint8_t *data, size_t dataLength)
{
	/* 	vector to contain the decoded string 	*/
	vector<uint8_t> *decodedData = new vector<uint8_t>;

	/*  iterator into the map containing our decoded translation    */
	unordered_map<char, uint8_t>::iterator mapKey;

	uint8_t decodedByte = 0x00;
	uint8_t encodedByte = 0x00;


	size_t currPos = 0;



	// WHILE we haven't reached the end of the encoded data
	// NOTE: according to the RFC, a valid encoded stream will always be 4 characters
	// per 24 bits (or 3 octets). Only the last two characters can be '=' which denote
	// the remaining bits of the 24 bit stream should be 0's.
	while (currPos <= dataLength)
	{

		decodedByte = 0x00;
		encodedByte = 0x00;

		// Get the FIRST encoded byte location
		mapKey = _decodingMap.find(data[currPos++]);


		// IF the map contains this key
		if (mapKey != _decodingMap.end())
		{
			
			encodedByte = mapKey->second;

			// Store its 6 bits in the first 6 bits of our decoded byte
			decodedByte |= ((encodedByte << 2) & 0xFC);


			// Get the SECOND encoded byte location
			mapKey = _decodingMap.find(data[currPos++]);

			if (mapKey != _decodingMap.end())
			{

				encodedByte = 0x00;
				encodedByte = mapKey->second;

				// Fill the remaining two bits of the decoded data
				decodedByte |= (encodedByte >> 4);

				decodedData->push_back(decodedByte);
				decodedByte = 0x00;


				// Get the remaining 4 bytes of our current index
				decodedByte |= ((encodedByte << 4) & 0xF0);


				// Get the THIRD encoded byte location
				mapKey = _decodingMap.find(data[currPos++]);

				// IF the map contains this key
				if (mapKey != _decodingMap.end())
				{

					encodedByte = 0x00;
					encodedByte = mapKey->second;

					// Store its first 4 bits in the last 4 positions of the decoded byte
					decodedByte |= ((encodedByte >> 2) & 0x0F);

					decodedData->push_back(decodedByte);
					decodedByte = 0x00;

					// Store the last 2 bits in the fresh byte
					decodedByte |= ((encodedByte << 6) & 0xC0);


					// Get the FOURTH encoded byte location
					mapKey = _decodingMap.find(data[currPos++]);

					if (mapKey != _decodingMap.end())
					{
						encodedByte = 0x00;
						encodedByte = mapKey->second;

						// Store the remaining 6 bits of the 24 bit total "Quantum" (as per the RFC)
						decodedByte |= (encodedByte & 0x3F);


						decodedData->push_back(decodedByte);
						decodedByte = 0x00;

					}
						// ELSE IF the character denoted padding we know the remaining bits are 0's
					else if (data[currPos - 1] == '=')
					{
						break;    // done decoding
					}
					else
					{
						cout << "Value not in map! (Base64Encoder::decode()" << endl;
					}

				}
					// ELSE IF the character denoted padding we know the remaining bits are 0's
				else if (data[currPos - 1] == '=')
				{
					break;    // done decoding
				}
				else
				{
					cout << "Value not in map! (Base64Encoder::decode()" << endl;
				}

			}
			else
			{
				cout << "Value not in map! (Base64Encoder::decode()" << endl;
			}
		}
		else
		{
			cout << "Value not in map! (Base64Encoder::decode()" << endl;
		}

	}// END WHILE ON 24 BIT SEGMENTS


	return decodedData;
}