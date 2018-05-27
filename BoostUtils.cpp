#include "stdafx.h"
#include "BoostUtils.h"

#include <iostream> 
#include <string>
#include <algorithm>
#include <iterator>
#include <cctype>
#include <boost/regex.hpp>

CBoostUtils::CBoostUtils()
{
}


CBoostUtils::~CBoostUtils()
{
}

std::vector<std::string> CBoostUtils::split(std::string str, std::string s) {
	std::vector<std::string> vec;

	boost::regex reg(s.c_str());
	boost::sregex_token_iterator it(str.begin(), str.end(), reg, -1);
	boost::sregex_token_iterator end;
	while (it != end) {
		vec.push_back(*it++);
	}

	return vec;
}
