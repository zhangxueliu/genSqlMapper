#include "stdafx.h"
#include "FileApi.h"
#include <direct.h>
#include <io.h>

bool assureDirExist(const char *fileName) {
	char sp1 = '\\'
		, sp2 = '/'
		;
	const char * p1 = strrchr(fileName, sp1);
	const char * p2 = strrchr(fileName, sp2);
	const char * pb = NULL == p1 ? p2 : p1;

	if ((NULL == pb) || (fileName == pb)) {
		return false;
	}

	char path[1024] = { 0 };
	const char *pCurPos = fileName;
	while (pCurPos = strchr(pCurPos, pb[0])) {

		memset(path, 0, sizeof(path) / sizeof(char));
		strncpy(path, fileName, pCurPos - fileName);
		if (-1 == (_access(path, 0))) {
			if (-1 == mkdir(path))
				return false;
		}

		pCurPos++;
	}

	return true;
}


FILE * g_OpenFile(const char *fileName, const char *mode) {
    FILE *file = NULL;
	assureDirExist(fileName);
    fopen_s(&file, fileName, mode);
    return file;
}


bool g_CloseFile(FILE *file) {
	return (0 == fclose(file));
}


size_t g_ReadFile(FILE *fp, void *buf, size_t size)    {
    return fread(buf, 1, size, fp);
}


size_t g_GetFileSize(const char *fileName)    {
	FILE * fp = NULL;

	fopen_s(&fp, fileName, "rb");
	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	fclose(fp);

	return    size;
}
