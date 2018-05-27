#include "stdafx.h"
#include "FileApi.h"
#include "FileOpr.h"
#include <string>


CFileOpr::CFileOpr(const char *_fileName) : fileName(_fileName)
{
}

CFileOpr::~CFileOpr()
{
}


bool CFileOpr::LoadFileByLineMark() {
	this->lines.clear();

	FILE * fp = NULL;
	const char * mode = "rb";
	fp = g_OpenFile(this->fileName.c_str(), mode);
	if (NULL == fp) {
		return false;
	}

	size_t size = g_GetFileSize(fileName.c_str());
	char *buffer = new char[size];
	if (size != g_ReadFile(fp, buffer, size)) {
		g_CloseFile(fp);
		return false;
	}

	g_CloseFile(fp);

	this->lines.clear();
	size_t count = 0;
	const size_t size_1 = size;
	for (size_t i = 0; i < size_1; ++i) {
		if ((buffer[i] == '\r') || (buffer[i] == '\n')) {
			buffer[i] = 0;
			count = 0;
		} else {
			if (count == 0) {
				lines.push_back(&buffer[i]);
			}
			++count;
		}
	}

	return true;
}


bool CFileOpr::SaveToFile() const {
	FILE* fp = NULL;
	fp = g_OpenFile(fileName.c_str(), "wb");
	if (NULL == fp){
		return false;
	}

	string aLine;
	const size_t size = this->lines.size();
	for (size_t i = 0; i < size; ++i) {
		aLine = this->lines[i];
		aLine += "\n";
		fwrite(aLine.c_str(), sizeof(char), aLine.length(), fp);
	}

	fclose(fp);

	return true;
}


void CFileOpr::AppendData(const string aLine) {
	if ("" != aLine) {
		lines.push_back(aLine);
	}
}
