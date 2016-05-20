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

#include "global_variables.h"
#include "cstring_utils.h"
#include "sdk_util_custom.h"

void pfnSetModel(edict_t *eEnt, const char* szModel)
{
	unprecache_list* ulList = GlobalVariables::g_ulUnprecacheList;
	switch(szModel[0])
	{
		case '*':
		{
			RETURN_META(MRES_IGNORED);
			break;
		}
		case 'm':
		{
			szModel+=7;
			if(ulList->modelExists(szModel))
			{
				RETURN_META(MRES_SUPERCEDE);
			}
			break;
		}
		case 's':
		{
			szModel+=8;
			if(ulList->spriteExists(szModel))
			{
				RETURN_META(MRES_SUPERCEDE);
			}
			break;
		}
	}
	RETURN_META(MRES_IGNORED);
}
int pfnPrecacheSound(const char* szSound)
{
	unprecache_list* ulList = GlobalVariables::g_ulUnprecacheList;
	if(ulList->soundExists(szSound))
	{
		RETURN_META_VALUE(MRES_SUPERCEDE, FALSE);
	}
	RETURN_META_VALUE(MRES_IGNORED, FALSE);
}

int pfnPrecacheModel(const char* szModel)
{
	unprecache_list* ulList = GlobalVariables::g_ulUnprecacheList;
	switch(szModel[0])
	{
		case 'm':
		{
			szModel+=7;
			if(ulList->modelExists(szModel))
			{
				RETURN_META_VALUE(MRES_SUPERCEDE, FALSE);
			}
			break;
		}
		case 's':
		{
			szModel+=8;
			if(ulList->spriteExists(szModel))
			{
				RETURN_META_VALUE(MRES_SUPERCEDE, FALSE);
			}
			break;
		}
	}
	RETURN_META_VALUE(MRES_IGNORED, FALSE);
}



void pfnEmitSound(edict_t *iEnt, int iChannel, const char *szSample, /*int*/float fVolume, float fAttenuation, int fFlags, int iPitch)
{
	unprecache_list* ulList = GlobalVariables::g_ulUnprecacheList;
	if(ulList->soundExists(szSample))
	{
		RETURN_META(MRES_SUPERCEDE);
	}
	RETURN_META(MRES_IGNORED);
}
void pfnEmitAmbientSound(edict_t *iEnt, float *fPos, const char *szSample, float fVol, float fAttenuation, int fFlags, int iPitch)
{
	unprecache_list* ulList = GlobalVariables::g_ulUnprecacheList;
	if(ulList->soundExists(szSample))
	{
		RETURN_META(MRES_SUPERCEDE);
	}
	RETURN_META(MRES_IGNORED);
}

int pfnModelIndex(const char *szModel)
{
	if(szModel[0]=='m')
	{
		szModel+=7;
		if(GlobalVariables::g_ulUnprecacheList->modelExists(szModel))
		{
			RETURN_META_VALUE(MRES_SUPERCEDE, FALSE);
		}
	}
	RETURN_META_VALUE(MRES_IGNORED, FALSE);
}

enginefuncs_t meta_engfuncs;
C_DLLEXPORT int GetEngineFunctions(enginefuncs_t *pengfuncsFromEngine, 
								   int *interfaceVersion)
{
	memset(&meta_engfuncs, 0, sizeof(enginefuncs_t));
	meta_engfuncs.pfnSetModel = pfnSetModel;
	meta_engfuncs.pfnPrecacheSound = pfnPrecacheSound;
	meta_engfuncs.pfnPrecacheModel = pfnPrecacheModel;
	meta_engfuncs.pfnEmitSound = pfnEmitSound;
	meta_engfuncs.pfnEmitAmbientSound = pfnEmitAmbientSound;
	meta_engfuncs.pfnModelIndex = pfnModelIndex;
	if(!pengfuncsFromEngine) {
		UTIL_LogError("[Error] GetEngineFunctions called with null pengfuncsFromEngine");
		return(FALSE);
	}
	else if(*interfaceVersion != ENGINE_INTERFACE_VERSION) {
		UTIL_LogError("[Error] GetEngineFunctions version mismatch; requested=%d ours=%d", *interfaceVersion, ENGINE_INTERFACE_VERSION);
		*interfaceVersion = ENGINE_INTERFACE_VERSION;
		return(FALSE);
	}
	memcpy(pengfuncsFromEngine, &meta_engfuncs, sizeof(enginefuncs_t));
	return(TRUE);
}
