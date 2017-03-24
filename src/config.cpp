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

#include "config.h"
#include "helper/string_utils.h"
#include <fstream>
#include <algorithm>
Config::Config(const std::shared_ptr<Logger> &logger)
	:
	logger_(logger),
	options_(),
	defaultOptions_(),
	configWasLoadedOnce_(false)
{
}

bool Config::addOption(const std::string &title, Config::OptionType type, const char* defaultValue)
{
	const std::string defValue = static_cast<std::string>(defaultValue);
	return addOptionVariant(title, type, Config::VariantType(defValue));
}

bool Config::addOption(const std::string &title, Config::OptionType type, const std::string &defaultValue)
{
	return addOptionVariant(title, type, Config::VariantType(defaultValue));
}

bool Config::deleteOption(const std::string &title)
{
	options_.erase(title);
	auto end = defaultOptions_.end();
	auto newEnd = std::remove_if(defaultOptions_.begin(), end, [&title](const decltype(defaultOptions_)::value_type &e) -> bool
	{
		if(e.first == title)
			return true;
		return false;
	});
	if(end == newEnd)
		return false;

	defaultOptions_.erase(newEnd, end);
	return true;
}

bool Config::addOption(const std::string &title, Config::OptionType type, const bool &defaultValue)
{
	return addOptionVariant(title, type, Config::VariantType(defaultValue));
}

bool Config::addOption(const std::string &title, Config::OptionType type, const int &defaultValue)
{
	return addOptionVariant(title, type, Config::VariantType(defaultValue));
}

bool Config::addOption(const std::string &title, Config::OptionType type, const VariantType &defaultValue)
{
	return addOptionVariant(title, type, defaultValue);
}

bool Config::addOptionVariant(const std::string &title, Config::OptionType type, const Config::VariantType &defaultValue)
{
	if(!checkVariant(type, defaultValue))
		return false;

	try {
		options_.at(title);
	}
	catch (std::exception&) {
		options_[title] = {type, defaultValue};
		defaultOptions_.push_back({title , defaultValue});
		return true;
	}
	return false;
}

std::pair<Config::OptionType, Config::VariantType> Config::getOption(const std::string &title) const
{
	return options_.at(title);
}

bool Config::setOption(const std::string &title, const Config::VariantType &newValue)
{
	decltype(options_)::iterator position = options_.find(title);
	if(position == options_.end())
		return false;

	const OptionType type = (*position).second.first;

	if(!checkVariant(type, newValue))
		return false;

	(*position).second.second = newValue;

	return true;
}

bool Config::setOption(const std::string &title, const char* newValue)
{
	const std::string newValueStr = static_cast<std::string>(newValue);
	return setOption(title, newValueStr);
}
bool Config::setOption(const std::string &title, const bool &newValue)
{
	return setOption(title, Config::VariantType(newValue));
}

bool Config::setOption(const std::string &title, const int &newValue)
{
	return setOption(title, Config::VariantType(newValue));
}

bool Config::setOption(const std::string &title, const std::string &newValue)
{
	return setOption(title, Config::VariantType(newValue));
}

void Config::loadConfig(const std::string &path)
{
	std::ifstream file;
	file.open(path);

	if(!file)
		file.open(path, std::ios::out | std::ios::in | std::ios::trunc);

	if(!file)
		throw std::runtime_error("Cannot open/create file: " + path);

	if(configWasLoadedOnce_)
		resetOptionsToDefaults();
	else
		configWasLoadedOnce_ = true;

	std::string line;
	while(std::getline(file, line))
	{
		logger_->debug(std::string(__FUNCTION__) + " Reading line" + line);
		this->readLine(line);
	}

	file.close();
}

std::shared_ptr<Logger> Config::getLogger() const
{
	return logger_;
}

std::shared_ptr<Logger> &Config::getLoggerRef()
{
	return logger_;
}

void Config::setLogger(const std::shared_ptr<Logger> &logger)
{
	logger_ = logger;
}

bool Config::checkVariant(const Config::OptionType type, const Config::VariantType &variant) const
{
	try
	{
		switch(type)
		{
			case Config::OptionType::Boolean:
				boost::get<bool>(variant);
				break;
			case Config::OptionType::Integer:
				boost::get<int>(variant);
				break;
			case Config::OptionType::String:
				boost::get<std::string>(variant);
				break;
		}
	}
	catch (std::exception&) {
		return false;
	}
	return true;
}

void Config::resetOptionsToDefaults()
{
	for(auto &curPair : defaultOptions_)
	{
		decltype (options_)::iterator position = options_.find(curPair.first);
		(*position).second.second = curPair.second;
	}
}


bool Config::readLine(const std::string &lineRef)
{
	std::string line = trim(removeComments(lineRef));
	if(line.empty())
	{
		logger_->debug(std::string(__FUNCTION__) + " Line is empty. Return.");
		return false;
	}
	std::string::size_type equalitySignPos = line.find("=");

	if(equalitySignPos == std::string::npos)
	{
		logger_->warning(std::string(__FUNCTION__) + " Equality sign not found. Return.");
		return false;
	}
	std::string left = trim(line.substr(0, equalitySignPos));
	std::string right = trim(line.substr(equalitySignPos + 1, std::string::npos));
	logger_->debug(std::string(__FUNCTION__) + " Left right: " + left + " = " + right);
	try
	{
		decltype(options_)::mapped_type &value = options_.at(left);

		const auto BooleanFunction = [&value, this](const std::string &checkString) -> bool
		{
			const auto assignBool = [&value, this](bool assignValue){ value.second = assignValue; };
			const std::unordered_map<std::string, std::function<void()>> switchMap =
			{{"true",std::bind(assignBool, true)},
			{"1",std::bind(assignBool, true)},
			{"yes",std::bind(assignBool, true)},
			{"yeah",std::bind(assignBool, true)},

			{"false",std::bind(assignBool, false)},
			{"0",std::bind(assignBool, false)},
			{"no",std::bind(assignBool, false)},
			{"mazdan",std::bind(assignBool, false)}};

			std::string localCopy;
			std::transform(checkString.begin(), checkString.end(), std::back_inserter(localCopy), ::tolower);
			try {
				switchMap.at(localCopy)();
			} catch (std::exception&) {
				logger_->error(std::string(__FUNCTION__) + " Exception occured. Return from BF");
				return false;
			}
			return true;
		};

		switch (value.first)
		{
			case Config::OptionType::Boolean:
			{
				bool result = BooleanFunction(right);
				logger_->debug(std::string(__FUNCTION__) + " BooleanFunction(" + right + ") result = " + std::to_string(result));
				break;
			}
			case Config::OptionType::Integer:
			{
				try {
					value.second = std::stoi(right);
				}
				catch (std::exception&) {
					value.second = int(0);
				}
				break;
			}
			case Config::OptionType::String:
			{
				logger_->debug(std::string(__FUNCTION__) + " Value.second = " + right);
				value.second = right;
				break;
			}
		}

	}
	catch (std::exception &e) {
		logger_->error(std::string(__FUNCTION__) + " Exception occured. " + e.what() + " Continue.");
		return false;
	}

	return true;
}
