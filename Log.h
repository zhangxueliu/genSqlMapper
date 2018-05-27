#ifndef __CLOG_H__
#define __CLOG_H__
#include "stdafx.h"
#include <string>

extern "C" void OutputDebugPrintf(const char* strOutputString, ...);
extern "C" void PrintComErrorInfo(_com_error &e);
extern "C" void PrintStringErrorInfo(const std::string& e);
#endif
