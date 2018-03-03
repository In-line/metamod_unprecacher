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

#include "module.h"

#include "helper/string_utils.h"

#include <fstream>
#include <utility>

#include <map>
#include <forward_list>

std::shared_ptr<Logger> Module::getLogger() const
{
	return logger_;
}

std::shared_ptr<Logger> &Module::getLoggerRef()
{
	return logger_;
}

void Module::setLogger(const std::shared_ptr<Logger> &logger)
{
	logger_ = logger;
}

Config Module::getConfig() const
{
	return config_;
}

Config &Module::getConfigRef()
{
	return config_;
}

void Module::setConfig(const Config &config)
{
	config_ = config;
}

void Module::analyzeLoggerStringPattern(const std::string &str)
{
	std::size_t categoryStart = static_cast<std::size_t>(Logger::CategoryType::Debug);
	std::size_t categoryMax = static_cast<std::size_t>(Logger::CategoryType::LastElement) + 1;
	for(std::size_t i =categoryStart
			;i < categoryMax
			;++i)
	{
		logger_->setCategoryDisplayed(i, false);
	}
	for(auto& c : str)
	{
		if(c >= 'a' && (std::size_t(c) <= ('a' +  categoryMax - 1) ) )
		{
			logger_->setCategoryDisplayed(std::size_t(c) - 'a',true);
		}
	}
}

Module::Module(const Logger::OutputType &outputType)
	:
		lastHitPoint_(),
		logger_(new Logger(outputType)),
		config_(logger_)
{
	config_.addOption(LOGGER_VERBOSITY, Config::OptionType::String, "de");
	logger_->setCategoryDisplayed(Logger::CategoryType::Error, true);
	logger_->setCategoryDisplayed(Logger::CategoryType::CriticalError, true);

	config_.addOption(LOGGER_OUTPUT, Config::OptionType::Integer, 3);
	logger_->setOutputType(Logger::OutputType::Both);
}

bool Module::readLine(std::string line)
{
	// Line has format: "SOME_PATH" "flags" "additional"

	logger_->debug(FNAME + " Line raw text: " + line);
	line = trim(removeComments(line));
	logger_->debug(FNAME + " Line text after: " + line);

	if (line.empty()) {
		logger_->debug(FNAME + " Line is empty. Continue to next.");
		return false;
	}

	std::vector<std::string> tokens = parseString(line, true);
	std::string path = tokens[0];

	std::string extension;
	try {
		// We know, that extensions have only 4 symbols at the end
		// Template: ".***"
		extension = path.substr(path.size() - 4, std::string::npos);
	}
	catch (std::out_of_range) {
		logger_->error(FNAME + " Cannot find extension. Continue.");
		return false;
	}

	const std::map<std::string, MAP> extensions =
		{
			{".mdl", MAP_MODELS},
			{".spr", MAP_SPRITES},
			{".wav", MAP_SOUNDS}
		};

	MAP currentMapType = MAP_MODELS;
	try {
		currentMapType = extensions.at(extension);
	}
	catch (std::out_of_range &) {
		logger_->error(FNAME + " Unrecognized extension. Continue");
		return false;
	}

	const std::string extensionPrefixes[MAP_SIZE] =
		{
			"models/", // MAP_MODELS
			"sprites/", // MAP_SPRITES
			"sound/" // MAP_SOUNDS
		};

	switch (currentMapType) {
		case MAP_SOUNDS:
			if (starts_with(path, extensionPrefixes[currentMapType])) // Remove "sound/"
				path = path.substr(extensionPrefixes[currentMapType].size(), std::string::npos);
			break;
		default:
			if (!starts_with(path, extensionPrefixes[currentMapType]))
				path = extensionPrefixes[currentMapType] + path;
			break;
	}

	UnprecacheOptions options;
	if (tokens.size() > 1)
	{
		try {
			options = UnprecacheOptions::stringPatternToOptions(tokens[1]);
		}
		catch (std::exception &e) {
			logger_->error(FNAME + " Exception when reading alphabet pattern " + e.what());
			logger_->error(FNAME + " Resetting unprecache options");
			options = UnprecacheOptions();
		}

		if (options.replace()) {
			if (tokens.size() > 2)
				options.setReplacedPath(tokens[2]);
			else {
				logger_->error(FNAME + " Can't find replace path, disabling replace function");
				options.setReplace(false);
			}
		}
	}

	// Insert our job result
	logger_->debug(FNAME + " Insert to mapType " + extensionPrefixes[currentMapType]);
	maps_[currentMapType][path] = options;
	return true;
}

void Module::loadLists(const std::string &path)
{
	loadListFromFile(path, [this](const std::string &line) {
		this->readLine(line);
	});
	this->revalidateEnds();
}

void Module::loadListFromFile(const std::string &path, std::function<void(const std::string &)> onLineRead)
{
	std::fstream inputFile;
	{
		logger_->debug(FNAME + " Try to open " + path);
		inputFile.open (
			path,
			std::ios::in
		);

		if(!inputFile)
		{
			logger_->warning(FNAME + " Cannot open, try to create");
			inputFile.open(path, std::ios::out | std::ios::in | std::ios::trunc);
		}

		if(!inputFile)
		{
			logger_->criticalError(FNAME + " Fail. Throwing exception...");
			throw std::ios::failure("Cannot create/open file: " + path);
		}
	}
	{
		logger_->debug(FNAME + " Start file reading");

		std::string line;
		while(getline(inputFile, line))
		{
			onLineRead(line);
		}
		logger_->debug(FNAME + " Input file close");
		inputFile.close();
	}
}

void Module::loadWhiteList(const std::string &path)
{
	std::forward_list<std::string> whiteList;
	loadListFromFile(path, [&whiteList, this](const std::string &line) {
		std::vector<std::string> tokens = parseString(trim(removeComments(line)));

		whiteList.push_front(tokens.front());
		logger_->debug(FNAME + "loaded whitelist line " + line);

		if(tokens.size() > 1) {
			logger_->warning(FNAME + " Ignoring additional tokens for " + line);
		}
	});

	for(const auto &line : whiteList) {
		maps_[MAP_MODELS].erase(line);
		maps_[MAP_SPRITES].erase(line);

		const std::string sound = "sound/";
		if(starts_with(line, sound)) {
			maps_[MAP_SOUNDS].erase(line.substr(sound.size(), std::string::npos));
		}
	}
	this->revalidateEnds();
}


void Module::clearLists()
{
	maps_[MAP_MODELS].clear();
	maps_[MAP_SPRITES].clear();
	maps_[MAP_SOUNDS].clear();
}

void Module::revalidateEnds()
{
	mapsEnds_[MAP_MODELS] = maps_[MAP_MODELS].end();
	mapsEnds_[MAP_SPRITES] = maps_[MAP_SPRITES].end();
	mapsEnds_[MAP_SOUNDS] = maps_[MAP_SOUNDS].end();
}

void Module::loadConfig(const std::string &path)
{
	config_.loadConfig(path);
	updateSettings();
}

void Module::updateSettings()
{
	analyzeLoggerStringPattern(boost::get<std::string>(config_.getOption(LOGGER_VERBOSITY).second));
	logger_->setOutputType(static_cast<Logger::OutputType>(boost::get<int>(config_.getOption(LOGGER_OUTPUT).second)));
}

const Module::UnprecacheMap& Module::getModelsMap() const
{
	return maps_[MAP_MODELS];
}

void Module::setModelsMap(const Module::UnprecacheMap &modelsMap)
{
	maps_[MAP_MODELS] = modelsMap;
	mapsEnds_[MAP_MODELS] = maps_[MAP_MODELS].end();
}

const Module::UnprecacheMap& Module::getSpritesMap() const
{
	return maps_[MAP_SPRITES];
}

void Module::setSpritesMap(const Module::UnprecacheMap &spritesMap)
{
	maps_[MAP_SPRITES] = spritesMap;
	mapsEnds_[MAP_SPRITES] = maps_[MAP_SPRITES].end();
}

const Module::UnprecacheMap& Module::getSoundsMap() const
{
	return maps_[MAP_SOUNDS];
}

void Module::setSoundsMap(const Module::UnprecacheMap &soundsMap)
{
	maps_[MAP_SOUNDS] = soundsMap;
	mapsEnds_[MAP_SOUNDS] = maps_[MAP_SOUNDS].end();
}

const UnprecacheOptions& Module::getLastHitPoint() const
{
	return *lastHitPoint_;
}

