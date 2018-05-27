#include "stdafx.h"
#include "Log.h"


void OutputDebugPrintf(const char* strOutputString, ...)
{
	char strBuffer[4096] = { 0 };
	va_list vlArgs;
	va_start(vlArgs, strOutputString);
	_vsnprintf_s(strBuffer, sizeof(strBuffer)-1, strOutputString, vlArgs);
	va_end(vlArgs);
	OutputDebugString(CA2W(strBuffer));
}


void PrintComErrorInfo(_com_error &e) {
	printf("Errorinfomation are as follows\n");
	printf("ErrorNo:%d\nError Message:%s\nError Source:%s\nError Description:%s\n", e.Error(), e.ErrorMessage(), (LPCTSTR)e.Source(), (LPCTSTR)e.Description());
}


void PrintStringErrorInfo(const std::string& e) {
	printf("%s\n", e.c_str());
}
