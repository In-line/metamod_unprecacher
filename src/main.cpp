/*
 * Ultimate Unprecacher
 * Copyright (c) 2017 Alik Aslanyan <cplusplus256@gmail.com>
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


#include "helper/external_api.h"
#include "module.h"
#include "helper/string_utils.h"
#include <dirent.h>
#include <fstream>
#include <ctime>
#include <iomanip>
Module* module = nullptr;

#ifdef _WIN32
extern "C" __declspec(dllexport) void __stdcall GiveFnptrsToDll(enginefuncs_t* pengfuncsFromEngine, globalvars_t *pGlobals);
#elif defined __linux__
extern "C" void GiveFnptrsToDll(enginefuncs_t* pengfuncsFromEngine, globalvars_t *pGlobals);
#endif

// Must provide at least one of these..
static META_FUNCTIONS gMetaFunctionTable = {
	nullptr,			// pfnGetEntityAPI		HL SDK; called before game DLL
	nullptr,			// pfnGetEntityAPI_Post		META; called after game DLL
	GetEntityAPI2,		// pfnGetEntityAPI2		HL SDK2; called before game DLL
	nullptr,			// pfnGetEntityAPI2_Post	META; called after game DLL
	nullptr,			// pfnGetNewDLLFunctions	HL SDK2; called before game DLL
	nullptr,			// pfnGetNewDLLFunctions_Post	META; called after game DLL
	GetEngineFunctions,	// pfnGetEngineFunctions	META; called before HL engine
	nullptr,			// pfnGetEngineFunctions_Post	META; called after HL engine
};

plugin_info_t Plugin_info = {
	META_INTERFACE_VERSION,
	"Ultimate Unprecacher",
	"Beta 2.0",
	"2016/05/18",
	"Alik Aslanyan <cplusplus256@gmail.com>",
	"https://github.com/in-line/metamod_unprecacher",
	"",
	PT_CHANGELEVEL,
	PT_CHANGELEVEL,
};

meta_globals_t *gpMetaGlobals;
gamedll_funcs_t *gpGamedllFuncs;
mutil_funcs_t *gpMetaUtilFuncs;

std::string *listPath;
std::string *configPath;
std::string *pluginDirPath;
std::string *mapName;

enum class ModuleFunctions
{
	SET_MODELF,
	PRECACHE_SOUNDF,
	PRECACHE_MODELF,
	EMIT_SOUNDF,
	EMIT_AMBIENT_SOUNDF,
	MODEL_INDEXF,
	LastElement = MODEL_INDEXF
};
#define MODULE_CHECKS "module_checks"
bool moduleFunctions[(std::size_t)ModuleFunctions::LastElement + 1];
void loadConfiguration();

C_DLLEXPORT int Meta_Query(const char * /*ifvers */, plugin_info_t **pPlugInfo, mutil_funcs_t *pMetaUtilFuncs)
{
	// Give metamod our plugin_info struct
	*pPlugInfo = &Plugin_info;
	// Get metamod utility function table.
	gpMetaUtilFuncs = pMetaUtilFuncs;
	return(TRUE);
}

C_DLLEXPORT int Meta_Attach(PLUG_LOADTIME /* now */,
                            META_FUNCTIONS *pFunctionTable, meta_globals_t *pMGlobals,
                            gamedll_funcs_t *pGamedllFuncs)
{

	if(!pMGlobals) {
		LOG_ERROR(PLID, "[Error] Meta_Attach called with nullptr pMGlobals");
		return(FALSE);
	}
	gpMetaGlobals = pMGlobals;
	if(!pFunctionTable) {
		LOG_ERROR(PLID, "[Error] Meta_Attach called with nullptr pFunctionTable");
		return(FALSE);
	}
	memcpy(pFunctionTable, &gMetaFunctionTable, sizeof(META_FUNCTIONS));
	gpGamedllFuncs = pGamedllFuncs;

	listPath = new std::string();
	configPath = new std::string();
	pluginDirPath = new std::string();
	mapName = new std::string();
	memset(&moduleFunctions[0], false, sizeof(ModuleFunctions) * sizeof(moduleFunctions[0]));
	*pluginDirPath = GET_PLUGIN_PATH(PLID);

	{
		size_t lastSlash;
		if( ( lastSlash = pluginDirPath->find_last_of("/") ) != std::string::npos )
		{
			pluginDirPath->erase(lastSlash, std::string::npos);
		}
	}

	*listPath = *pluginDirPath + "/" + "list.ini";
	*configPath = *pluginDirPath + "/" + "config.ini";

	module = new Module();
	module->getConfigRef().addOption(MODULE_CHECKS, Config::OptionType::String, "abcde");
	return(TRUE);
}

C_DLLEXPORT int Meta_Detach(PLUG_LOADTIME, PL_UNLOAD_REASON)
{
	delete module;
	delete listPath;
	delete configPath;
	delete pluginDirPath;
	delete mapName;
	return(TRUE);
}
inline void handleUnprecacheOptions(edict_t *eEnt, std::function<void(const char*)> replaceFunction)
{
	const UnprecacheOptions &options = module->getLastHitPoint();
	if(options.isNotDefault())
	{
		if(options.deleteEntity())
		{
			if(33>ENTINDEX(eEnt) || !options.notDeleteHuman())
				eEnt->v.flags = eEnt->v.flags | FL_KILLME;
		}
		else if(options.replace())
		{
			replaceFunction(options.replacedPath().c_str());
		}
	}
}
#include <sstream>
void loadConfiguration()
{
	*mapName = std::string(STRING(gpGlobals->mapname));
	mkdir((*pluginDirPath + "/" + "logs").c_str(), 0700);

	{
		memset(&moduleFunctions[0], false, sizeof(ModuleFunctions) * sizeof(moduleFunctions[0]));
		module->loadConfig(*configPath);
		std::string functionsStr = boost::get<std::string>(module->getConfigRef().getOption(MODULE_CHECKS).second);
		for(auto &c : functionsStr)
		{
			constexpr std::size_t size = static_cast<std::size_t>(ModuleFunctions::LastElement);
			if(c>='a' && std::size_t(c)<=('a'+size))
			{
				moduleFunctions[c - 'a'] = true;
			}
		}
	}

	module->getLoggerRef()->setConsoleFunction([&](const std::string &str)
	{
		const std::string logPrefix = "[Ultimate Unprecacher] ";
		std::time_t curTime = std::time(nullptr);
		auto localTime = std::localtime(&curTime);
		SERVER_PRINT((logPrefix + *mapName + str_put_time(localTime, " %H:%M:%S - ") + str + "\n" ).c_str());
	});

	module->getLoggerRef()->setFileFunction([&](const std::string &str)
	{
		const std::string logPrefix = "[Ultimate Unprecacher] ";
		std::time_t curTime = std::time(nullptr);
		auto localTime = std::localtime(&curTime);

		std::ofstream logFile((*pluginDirPath + "/" + "logs" + "/" +
		                       str_put_time(localTime, "L%d%m%y") + ".log"), std::ios::out | std::ios::app);
		if(!logFile)
			module->getLoggerRef()->consoleFunction()(logPrefix + "[Error while writing in file] " + str);
		else
		{
			logFile << logPrefix << mapName;			
			char hms[64];
			if(strftime(hms, sizeof(hms), " %H:%M:%S - ", localTime) > 0) 
				logFile << hms;
			logFile << str << std::endl;
		}
	});

	module->clearLists();
	module->loadLists(*listPath);

	DIR* mapsDir = nullptr;
	std::string mapDirPath = *pluginDirPath + "/" + "maps";

	mapsDir = opendir(mapDirPath.c_str()); // Try to open directory

	if (!mapsDir && ENOENT == errno) // If it is no directory, create one
	{
		if(mkdir(mapDirPath.c_str(), 0700) == 0) // If creation successful reopen it
		{
			mapsDir = opendir(mapDirPath.c_str());
		}
	}


	if(mapsDir)
	{
		struct dirent *dirEnt;

		const std::string prefix = "prefix-";
		while ((dirEnt = readdir(mapsDir)) != NULL)
		{
			struct stat sb;
			const std::string currentFileName = dirEnt->d_name;
			const std::string currentFile = mapDirPath + "/" + currentFileName;
			if(stat(currentFile.c_str(), &sb) == 0 && !S_ISDIR(sb.st_mode)) // Check if path is file
			{
				if(starts_with(currentFileName, prefix))
				{
					std::string mapPrefix;
					try
					{
						mapPrefix = currentFileName.substr(prefix.size(), currentFileName.size() - (prefix.size() + /*.ini*/4));
					} catch (std::exception&) { }
					if(starts_with(*mapName, mapPrefix))
					{
						module->loadLists(currentFile);
					}
				}
				else if(currentFileName == (*mapName + ".ini"))
				{
					module->loadLists(currentFile);
				}
			}
		}
		closedir(mapsDir);
	}
}

void pfnSetModel(edict_t *eEnt, const char* szModel)
{
	bool result = false;
	switch(szModel[0])
	{
		case 'm':
			result = module->checkModel(szModel);
			break;
		case 's':
			result = module->checkSprite(szModel);
			break;
	}
	if(result == true)
	{
		handleUnprecacheOptions(eEnt, [eEnt](const char* newPath)
		{
			SET_MODEL(eEnt, newPath);
		});
		RETURN_META(MRES_SUPERCEDE);
	}
	RETURN_META(MRES_IGNORED);
}

void pfnEmitSound(edict_t *iEnt, int iChannel, const char *szSample, /*int*/float fVolume, float fAttenuation, int fFlags, int iPitch)
{
	if(moduleFunctions[(std::size_t)ModuleFunctions::EMIT_SOUNDF] && module->checkSound(szSample))
	{
		handleUnprecacheOptions(iEnt,[&] (const char* newPath)
		{
			EMIT_SOUND_DYN2(iEnt, iChannel, newPath, fVolume, fAttenuation, fFlags, iPitch);
		});
		RETURN_META(MRES_SUPERCEDE);
	}
	RETURN_META(MRES_IGNORED);
}

void pfnEmitAmbientSound(edict_t *iEnt, float *fPos, const char *szSample, float fVol, float fAttenuation, int fFlags, int iPitch)
{
	if(moduleFunctions[(std::size_t)ModuleFunctions::EMIT_AMBIENT_SOUNDF] && module->checkSound(szSample))
	{
		handleUnprecacheOptions(iEnt,[&] (const char* newPath)
		{
			EMIT_AMBIENT_SOUND(iEnt, fPos, newPath, fVol, fAttenuation, fFlags, iPitch);
		});
		RETURN_META(MRES_SUPERCEDE);
	}
	RETURN_META(MRES_IGNORED);
}

int pfnPrecacheSound(const char* szSound)
{
	if(moduleFunctions[(std::size_t)ModuleFunctions::PRECACHE_SOUNDF] && module->checkSound(szSound))
	{
		RETURN_META_VALUE(MRES_SUPERCEDE, FALSE);
	}
	RETURN_META_VALUE(MRES_IGNORED, FALSE);
}

int pfnPrecacheModel(const char* szModel)
{
	if(!moduleFunctions[(std::size_t)ModuleFunctions::PRECACHE_MODELF])
		RETURN_META_VALUE(MRES_IGNORED, false);

	bool result = false;
	switch(szModel[0])
	{
		case 'm':
			result = module->checkModel(std::string(szModel));
			break;
		case 's':
			result = module->checkSprite(std::string(szModel));
			break;
	}
	if(result == true)
	{
		RETURN_META_VALUE(MRES_SUPERCEDE, false);
	}
	RETURN_META_VALUE(MRES_IGNORED, false);
}

int pfnModelIndex(const char *szModel)
{
	if(moduleFunctions[(std::size_t)ModuleFunctions::MODEL_INDEXF] && module->checkModel(std::string(szModel)))
	{
		RETURN_META_VALUE(MRES_SUPERCEDE, FALSE);
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
		LOG_ERROR(PLID, "[Error] GetEngineFunctions called with nullptr pengfuncsFromEngine");
		return(FALSE);
	}
	else if(*interfaceVersion != ENGINE_INTERFACE_VERSION) {
		LOG_ERROR(PLID, "[Error] GetEngineFunctions version mismatch; requested=%d ours=%d", *interfaceVersion, ENGINE_INTERFACE_VERSION);
		*interfaceVersion = ENGINE_INTERFACE_VERSION;
		return(FALSE);
	}
	memcpy(pengfuncsFromEngine, &meta_engfuncs, sizeof(enginefuncs_t));
	return(TRUE);
}
bool g_calledSpawn = false;

void pfnServerDeactivate()
{
	if (!g_calledSpawn)
		RETURN_META(MRES_IGNORED);


	g_calledSpawn = false;

	RETURN_META(MRES_IGNORED);
}

int pfnSpawn(edict_t*)
{
	if(g_calledSpawn)
	{
		RETURN_META_VALUE(MRES_IGNORED, 0);
	}
	loadConfiguration();
	for(short i = 1; i<=2; ++i)
	{
		auto &map = i==1 ? module->getModelsMap() : module->getSpritesMap();
		for(auto& pairValue : map)
		{
			if(pairValue.second.replace())
			{
				PRECACHE_MODEL((char*)STRING(ALLOC_STRING(pairValue.second.replacedPath().c_str())));
			}
		}
	}

	{
		auto &soundMap = module->getSoundsMap();
		for(auto& pairValue : soundMap)
		{
			if(pairValue.second.replace())
			{
				std::string replacedPath;
				{
					const std::string &originalRPath = pairValue.second.replacedPath();
					const std::string sound = "sound/";
					replacedPath = originalRPath.substr( starts_with(originalRPath, sound) ? sound.size() : 0, std::string::npos);
				}
				PRECACHE_SOUND((char*)STRING(ALLOC_STRING(replacedPath.c_str())));
			}
		}
	}
	g_calledSpawn = true;

	RETURN_META_VALUE(MRES_IGNORED, 0);
}

DLL_FUNCTIONS gFunctionTable;
C_DLLEXPORT int GetEntityAPI2(DLL_FUNCTIONS *pFunctionTable, int* /*interfaceVersion*/)
{
	memset(&gFunctionTable, 0, sizeof(DLL_FUNCTIONS));

	gFunctionTable.pfnSpawn = pfnSpawn;
	gFunctionTable.pfnServerDeactivate = pfnServerDeactivate;
	memcpy(pFunctionTable, &gFunctionTable, sizeof(DLL_FUNCTIONS));

	return 1;
}

#include <h_export.h>

// From SDK dlls/h_export.cpp:

//! Holds engine functionality callbacks
enginefuncs_t g_engfuncs;
globalvars_t  *gpGlobals;

// Receive engine function table from engine.
// This appears to be the _first_ DLL routine called by the engine, so we
// do some setup operations here.

C_DLLEXPORT void WINAPI GiveFnptrsToDll( enginefuncs_t* pengfuncsFromEngine, globalvars_t *pGlobals )
{
	memcpy(&g_engfuncs, pengfuncsFromEngine, sizeof(enginefuncs_t));
	gpGlobals = pGlobals;
}
