/* Most of functions from stackoverflow */

#include "cstring_utils.h"
#include <stdio.h>
#include <cstring>
#include <ctype.h>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <iomanip>
#include <stdarg.h>
void UTIL_RemoveComments(char *buf)
{
	for(unsigned short i = 0;i<2;i++)
	{
		char *f = strstr(buf, i==0 ? "//" : ";");
		if(f) *f = '\0';
	}
}

void trim(char *str)
{
	register char *start, *end;

	for (start = str; *start; start++)
	{
		if (!isspace((unsigned char)start[0]))
			break;
	}

	for (end = start + strlen(start); end > start + 1; end--)
	{
		if (!isspace((unsigned char)end[-1]))
			break;
	}

	*end = 0;


	if (start > str)
		memmove(str, start, (end - start) + 1);
}


bool str_is_empty(char *szStr,bool isSpaceEmpty)
{
	for(register int i = 0;;i++)
	{
		if(szStr[i]==0) return true;
		if(!isspace(szStr[i]) && (!isSpaceEmpty || (isSpaceEmpty && szStr[i]!=' ') ))
		{
			return false;
		}
	}
	return true;
}

char* stradd(const char* a, const char* b)
{
	size_t len = strlen(a) + strlen(b);
	char *ret = (char*)malloc(len * sizeof(char) + 1);
	*ret = '\0';
	return strcat(strcat(ret, a) ,b);
}

char* str_copy(const char* s)
{
	char* szBuffer = new char[strlen(s) + 1];
	strcpy(szBuffer,s);
	return szBuffer;
}



void str_substr2(char sub[], char s[], int p, int l)
{
	register int c = 0;

	while (c < l)
	{
		sub[c] = s[p+c-1];
		c++;
	}
	sub[c] = '\0';
}


struct MatchPathSeparator
{
	inline bool operator()( char ch ) const
	{
#ifdef _WIN32
		return ch == '/';
#else
		return ch == '\\' || ch == '/';
#endif
	}
};

char* get_path(char *szPathToFile)
{
	int iLen = strlen(szPathToFile);
	char* szReturn = new char[iLen+1];
	strcpy(szReturn, szPathToFile);
	for(unsigned int i = iLen;i;--i)
	{
		if(MatchPathSeparator()(szReturn[i]))
		{
			szReturn[i+1] = '\0';
			break;
		}
	}
	return szReturn;
}

void replace_one_char(char *szStr, char what, char with)
{
	for(register unsigned int i = 0;szStr[i]!=0;++i)
	{
		if(szStr[i] == what) szStr[i] = with;
	}
}



char* get_timestring(char* sptemp)
{
	time_t td = std::time(NULL);
	tm* lt = std::localtime(&td);

	char szDate[512];
	std::strftime(szDate, 511, sptemp, lt);
	return str_copy(szDate);
}
#ifdef _WIN32
int vasprintf(char **strp, const char *fmt, va_list ap)
{
	va_list ap1;
	size_t size;
	char *buffer;

	va_copy(ap1, ap);
	size = vsnprintf(NULL, 0, fmt, ap1) + 1;
	va_end(ap1);
	buffer = (char*) calloc(1, size);

	if (!buffer)
		return -1;

	*strp = buffer;

	return vsnprintf(buffer, size, fmt, ap);
}

int asprintf(char **strp, const char *fmt, ...)
{
	int error;
	va_list ap;

	va_start(ap, fmt);
	error = vasprintf(strp, fmt, ap);
	va_end(ap);

	return error;
}
#endif

bool starts_with(const char* string, const char* prefix)
{
	while(*prefix)
	{
		if(*prefix++ != *string++)
			return false;
	}

	return true;
}

void str_remove_first_chars(char *string, unsigned int iCount)
{

	unsigned int iDestChar = 0;
	for(unsigned int iCopyChar = iCount;string[iCopyChar]!=0;++iDestChar, ++iCopyChar)
	{
		string[iDestChar] = string[iCopyChar];
	}
	string[iDestChar] = '\0';
}

bool str_nends_with(char *postfix, char *string, int iLen)
{
	string+=iLen - strlen(postfix);
	while(*postfix)
	{
		if(*postfix++ != *string++)
			return false;
	}
	return true;
}
