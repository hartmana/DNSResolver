//
//  StringUtils.cpp
//  DNSResolver
//
//  Created by Aaron Hartman on 10 Nov 14.
//  Copyright (c) 2014 Aaron Hartman. All rights reserved.
//

#include "StringUtils.h"

#include <regex>
#include <sstream>
#include <stdexcept>
#include <unistd.h>


using std::cout;
using std::cerr;
using std::endl;
using std::stringstream;
using std::runtime_error;
using std::regex;
using std::sregex_token_iterator;


///
/// Function name: split
/// Description: Splits a string based upon delimeter and returns a vector of the results
/// Parameters: Reference to the string to be split
///             char denoting the deliminator
///             vector the results will be stored in
///             struct suckaddr_storage reference
/// Returns: vector filled with split strings
///
vector<string> &StringUtils::split(const string &s, const char delim, vector<string> &elems)
{

	stringstream ss(s);
	string item;
	while (getline(ss, item, delim))
	{
		elems.push_back(item);
	}
	return elems;
}


///
/// Function name: split
/// Description: Wrapper function to split. Allows the caller to not have to pass
/// a vector if not necessary. Splits a string based upon delimeter and returns a vector of the results.
/// Parameters: Reference to the string to be split
///             char denoting the deliminator
///             
/// Returns: vector filled with split strings
///
vector<string> StringUtils::split(const string &s, const char delim)
{

	vector<string> elems;
	split(s, delim, elems);
	return elems;
}
