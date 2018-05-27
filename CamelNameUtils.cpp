#include "stdafx.h"
#include <iostream>
#include <algorithm>
#include "CamelNameUtils.h"
#include "BoostUtils.h"

using namespace std;

CamelNameUtils::CamelNameUtils()
{
}


CamelNameUtils::~CamelNameUtils()
{
}

string CamelNameUtils::Capitalize(string & in, string & out) {
	transform(in.begin(), in.begin() + 1, in.begin(), ::toupper);
	transform(in.begin() + 1, in.end(), in.begin() + 1, ::tolower);
	return in;
}

string CamelNameUtils::UnderScore2camel(string & in, string & out) {
	std::vector<std::string> vec = CBoostUtils::split(in, "_");
	for (int i = 0, size = (int)vec.size(); i < size; i++) {

		if (0 == i) {
			transform(vec[i].begin(), vec[i].begin() + 1, vec[i].begin(), ::tolower);
		} else {
			transform(vec[i].begin(), vec[i].begin() + 1, vec[i].begin(), ::toupper);
		}

		transform(vec[i].begin() + 1, vec[i].end(), vec[i].begin() + 1, ::tolower);
		std::cout << vec[i] << std::endl;
	}

	out.clear();
	for (int i = 0, size = (int)vec.size(); i < size; i++) {
		out += vec[i];
	}

	return out;
}

string CamelNameUtils::Camel2UnderScore(string & in, string & out) {
	return in;
}
