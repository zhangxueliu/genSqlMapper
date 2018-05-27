#ifndef __FILE_API_H__
#define __FILE_API_H__

extern "C" FILE * g_OpenFile(const char *fileName, const char *mode);
extern "C" bool g_CloseFile(FILE *file);
extern "C" size_t g_ReadFile(FILE    *fp, void *buf, size_t size);
extern "C" size_t g_GetFileSize(const char *fileName);

#endif
