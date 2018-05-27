#pragma once
#include <string>
#include <vector>

using namespace std;

class CFileOpr
{
public:
	CFileOpr(const char *_fileName);
	~CFileOpr();

public:
	bool LoadFileByLineMark();
	bool SaveToFile() const;	
	void AppendData(const string aLine);

private:
	vector<string> lines;
	string fileName;
};
