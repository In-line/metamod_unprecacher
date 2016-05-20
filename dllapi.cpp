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

#include <dllapi.h>
#include <meta_api.h>
#include <pm_defs.h>
#include "sdk_util_custom.h"
#include "config_file.h"
#include "global_variables.h"

void PM_PlaySoundBlock( int channel, const char *sample, float volume, float attenuation, int fFlags, int pitch )
{
	return;
}
void pfnPM_Move ( struct playermove_s *ppmove, int server )
{
	if(!config_file::cfgBlockSound)
	{
		RETURN_META(MRES_IGNORED);
	}
	ppmove->PM_PlaySound = PM_PlaySoundBlock;
	MDLL_PM_Move(ppmove, server);
	RETURN_META(MRES_SUPERCEDE);
}

void pfnServerDeactivate()
{
	config_file::LoadCfg(GlobalVariables::g_szConfigPath);
	GlobalVariables::g_ulUnprecacheList->loadFromFile(GlobalVariables::g_szIniPath);
}

DLL_FUNCTIONS gFunctionTable;
C_DLLEXPORT int GetEntityAPI2(DLL_FUNCTIONS *pFunctionTable, 
		int *interfaceVersion)
{
	memset(&gFunctionTable, 0, sizeof(gFunctionTable));
	gFunctionTable.pfnPM_Move = pfnPM_Move;
	gFunctionTable.pfnServerDeactivate = pfnServerDeactivate;
	if(!pFunctionTable) {
		UTIL_LogError("[Error] GetEntityAPI2 called with null pFunctionTable");
		return(FALSE);
	}
	else if(*interfaceVersion != INTERFACE_VERSION) {
		UTIL_LogError("[Error] GetEntityAPI2 version mismatch; requested=%d ours=%d", *interfaceVersion, INTERFACE_VERSION);
		*interfaceVersion = INTERFACE_VERSION;
		return(FALSE);
	}
	memcpy(pFunctionTable, &gFunctionTable, sizeof(DLL_FUNCTIONS));
	return(TRUE);
}
