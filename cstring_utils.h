#ifndef CSTRING_UTILS_H
#define CSTRING_UTILS_H

void UTIL_RemoveComments(char *buf);
void trim(char *str);
bool str_is_empty(char *szStr,bool isSpaceEmpty = true);

char* str_copy(const char* s);
void str_substr2(char sub[], char s[], int p, int l);
void replace_one_char(char *szStr, char what, char with);
char* get_timestring(char* sptemp);
#ifdef _WIN32
#include <cstdarg>
int vasprintf(char **strp, const char *fmt, va_list ap);
int asprintf(char **strp, const char *fmt, ...);
#endif
bool starts_with(const char* string, const char* prefix);
void str_remove_first_chars(char *string, int unsigned iCount);
bool str_nends_with(char *postfix, char *string, int iLen);
#endif // CSTRING_UTILS_H
