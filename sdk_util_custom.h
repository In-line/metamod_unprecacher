#ifndef SDK_UTIL_CUSTOM_H
#define SDK_UTIL_CUSTOM_H
#include <extdll.h>
#include <meta_api.h>
void UTIL_LogToFile(char* szFileName, const char *fmt, ...);
void UTIL_LogError(const char *fmt, ...);
bool CreateDirectoryIfNotExists(char *szPath);
#include <sdk_util.h>


#endif // SDK_UTIL_CUSTOM_H
