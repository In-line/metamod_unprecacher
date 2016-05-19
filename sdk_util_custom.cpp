/*
 * Ultimate Unprecacher
 * Copyright (c) 2016 Alik Aslanyan <cplusplus256@gmail.com>
 *
 *
 *
 *    This program is free software; you can redistribute it and/or modify it
 *    under the terms of the GNU General Public License as published by the
 *    Free Software Foundation; either version 2 of the License, or (at
 *    your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful, but
 *    WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software Foundation,
 *    Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *    In addition, as a special exception, the author gives permission to
 *    link the code of this program with the Half-Life Game Engine ("HL
 *    Engine") and Modified Game Libraries ("MODs") developed by Valve,
 *    L.L.C ("Valve").  You must obey the GNU General Public License in all
 *    respects for all of the code used other than the HL Engine and MODs
 *    from Valve.  If you modify this file, you may extend this exception
 *    to your version of the file, but you are not obligated to do so.  If
 *    you do not wish to do so, delete this exception statement from your
 *    version.
 *
 */

#include <extdll.h>
#include <meta_api.h>
#include "osdep.h"
#include "cstring_utils.h"
#include "global_variables.h"
#include <stdio.h>
#include <iomanip>

#include "sdk_util_custom.h"
char g_szLogString[1024];
void UTIL_LogToFile(char* szFileName, const char *fmt, ...)
{
	va_list		argptr;
	va_start ( argptr, fmt );
	vsnprintf ( g_szLogString, sizeof(g_szLogString), fmt, argptr );
	va_end   ( argptr );

	char* szFilePath = new char[strlen(GlobalVariables::g_szDLLDirPath) + 12 + 30 + 1];
	char* szLogFile = get_timestring("_%Y%m%d.log");
	sprintf(szFilePath, "%slogs/%s%s", (GlobalVariables::g_szDLLDirPath), szFileName, szLogFile);
	FILE* hFile = fopen(szFilePath, "a+");
	delete[] szFilePath;
	delete[] szLogFile;
	if(hFile == NULL)
	{
		char szError[256];
		sprintf(szError, "Error fopen: %s\n", strerror(errno));
		SERVER_PRINT(szError);
		clearerr(hFile);
		return;
	}
    
	fprintf(hFile, g_szLogString);
	if(ferror(hFile))
	{
		char szError[256];
		sprintf(szError, "Error fprintf: %s\n", strerror(errno));
		SERVER_PRINT(szError);
		clearerr(hFile);
		return;
	}
	fclose(hFile);

}

char* UTIL_GetLog(const char *fmt, ...)
{
	va_list			argptr;
	static char		temp[1024];
	
	va_start ( argptr, fmt );
	vsnprintf ( temp, sizeof(temp), fmt, argptr );
	va_end	( argptr );
	
	char* szDateTime = get_timestring("%d\\%m\\%Y %X");
	sprintf(g_szLogString,"[Metamod Ultimate Unprecacher] [%s] : %s\n",szDateTime,temp);
	delete[] szDateTime;
	
	return str_copy(g_szLogString);
	}

void UTIL_LogError(const char *fmt, ...)
{
	va_list			argptr;
	
	va_start ( argptr, fmt );
	vsnprintf ( g_szLogString, sizeof(g_szLogString), fmt, argptr );
	va_end   ( argptr );
	
	char*  szStr = UTIL_GetLog(g_szLogString);
	strcpy(g_szLogString, szStr);
	SERVER_PRINT(szStr);
	UTIL_LogToFile("error", g_szLogString);
	ALERT( at_error, szStr );
	delete[] szStr;
}
