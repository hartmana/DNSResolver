//
//  StringUtils.h
//  DNSResolver
//
//  Created by Aaron Hartman on 10 Nov 14.
//  Copyright (c) 2014 Aaron Hartman. All rights reserved.
//

#ifndef __DNSResolver__StringUtils__
#define __DNSResolver__StringUtils__


#include <iostream>
#include <vector>
#include <string.h>


using std::string;
using std::vector;


///
/// Class to provide abstracted functionality for strings
///
class StringUtils
{

private:


public:

	StringUtils()
	{
	};

	vector<string> &split(const string &s, const char delim, vector<string> &elems);

	vector<string> split(const string &s, const char delim);


};

#endif /* defined(__DNSResolver__StringUtils__) */
