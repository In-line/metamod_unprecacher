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
#include "config_file.h"
#include <stdio.h>
#include "cstring_utils.h"
#include <cstring>
#include "sdk_util_custom.h"
bool config_file::cfgBlockSound;
void config_file::LoadCfg(char *szPathToCfg)
{
	ResetAllVars();
	FILE* hFile = fopen(szPathToCfg,"a+");
	if(hFile == NULL)
	{
		UTIL_LogError("[Error] Cannot open config file %s", szPathToCfg);
		return;
	}

	char *szValue = NULL;
	char szBuffer[256];
	while(!feof(hFile))
	{
		if(fgets(szBuffer,sizeof(szBuffer),hFile) == NULL)
		{
			continue;
		}
		UTIL_RemoveComments(szBuffer);
		trim(szBuffer);

		if(!str_is_empty(szBuffer))
		{
			if((szValue = ParseCvar(szBuffer, "block_pm_move_sounds"))!=NULL)
			{
				trim(szValue);
				if(strstr(szValue,"true"))
					config_file::cfgBlockSound = true;
			}
			else
			{
				UTIL_LogError("[Error] unprecacher.cfg contents garbage data");
			}
		}
	}
	fclose(hFile);
}

void config_file::ResetAllVars()
{
	config_file::cfgBlockSound = false;
}

inline char* config_file::ParseCvar(char *szBuffer, char *szVarName)
{
	char* szStr = NULL;

	szStr = strtok(szBuffer,"=");
	if(szStr == NULL || str_is_empty(szStr)) return false;

	if(( szStr =strstr(szBuffer,szVarName)) != NULL)
	{
		if((szStr = strtok (NULL, "=")) != NULL)
		{
			trim(szStr);
			if(!str_is_empty(szStr))
			{
				return szStr;
			}
		}
	}
	return NULL;
}
