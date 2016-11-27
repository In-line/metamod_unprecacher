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

#ifndef MODULE_H
#define MODULE_H

#include <string>
#include <algorithm>


#include "unprecacheoptions.h"
#include "logger.h"
#include "config.h"

#include <unordered_map>
#include <string>
#include <memory>

#include <functional>
#pragma GCC diagnostic warning "-Wzero-as-null-pointer-constant"

class Module
{
private:
	typedef std::unordered_map<std::string, UnprecacheOptions> UnprecacheMap;
	enum MAP
	{
		MAP_START = 0,
		MAP_MODELS = 0,
		MAP_SPRITES = 1,
		MAP_SOUNDS = 2,
		MAP_SIZE = 3,
	};

	inline bool checkPathInMap(const std::string &path, MAP mapType)
	{
		const UnprecacheMap::const_iterator &result = maps_[mapType].find(path);
		if(result != mapsEnds_[mapType])
		{
#ifdef _DEBUG
			logger_->debug(std::string(__FUNCTION__) + " Match. " + path + " -> " + result->first);
#endif
			lastHitPoint_ = result->second;
			return true;
		}
		return false;
	}

	UnprecacheMap maps_[MAP_SIZE];
	UnprecacheMap::const_iterator mapsEnds_[MAP_SIZE];


	UnprecacheOptions lastHitPoint_;
	std::shared_ptr<Logger> logger_;
	Config config_;

#define LOGGER_VERBOSITY "logger_verbosity"
#define LOGGER_OUTPUT "logger_output"
public:
	Module(const Logger::OutputType &outputType = Logger::OutputType::NoOutput);
	Module(const Module&) = delete;
	Module(const Module&&) = delete;
	const Module& operator =(const Module&) = delete;
	const Module&& operator =(const Module&&) = delete;

	inline bool checkSprite(const std::string &path) { return checkPathInMap(path, MAP_SPRITES); }
	inline bool checkModel(const std::string &path) { return checkPathInMap(path, MAP_MODELS); }
	inline bool checkSound(const std::string &path){ return checkPathInMap(path, MAP_SOUNDS); }

	inline bool checkSprite(const char* path) { return this->checkSprite(std::string(path)); }
	inline bool checkModel(const char* path) { return this->checkModel(std::string(path)); }
	inline bool checkSound(const char* path) { return this->checkSound(std::string(path)); }

	bool readLine(const std::string &lineRef);
	void loadLists(const std::string &path);
	void clearLists();
	void revalidateEnds();

	void loadConfig(const std::string &path);
	void updateSettings();

	const UnprecacheMap& getModelsMap() const;
	void setModelsMap(const UnprecacheMap &modelsMap);

	const UnprecacheMap& getSpritesMap() const;
	void setSpritesMap(const UnprecacheMap &spritesMap);

	const UnprecacheMap& getSoundsMap() const;
	void setSoundsMap(const UnprecacheMap &soundsMap);

	const UnprecacheOptions &getLastHitPoint() const;

	std::shared_ptr<Logger> getLogger() const;
	std::shared_ptr<Logger> &getLoggerRef();
	void setLogger(const std::shared_ptr<Logger> &logger);

	Config getConfig() const;
	Config &getConfigRef();
	void setConfig(const Config &config);

	void analyzeLoggerVerbosityString(const std::string &str);
};

#endif // MODULE_H
