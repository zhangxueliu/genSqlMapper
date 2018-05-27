#pragma once

#include <string>
#include <vector>

class CBoostUtils
{
public:
	CBoostUtils();
	~CBoostUtils();

public:
	static std::vector<std::string> CBoostUtils::split(std::string str, std::string s);

};
