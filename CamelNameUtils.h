#pragma once

#include <string>

using namespace std;

class CamelNameUtils
{
protected:
	CamelNameUtils();
	~CamelNameUtils();

public:
	static string Capitalize(string & in, string & out);
	static string UnderScore2camel(string & in, string & out);
	static string Camel2UnderScore(string & in, string & out);
};
