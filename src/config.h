/*
 * Ultimate Unprecacher
 * Copyright (c) 2018 Alik Aslanyan <cplusplus256@gmail.com>
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

#pragma once
#ifndef CONFIG_H
#define CONFIG_H

#include <vector>
#include <string>
#include <unordered_map>
#include <boost/variant.hpp>


#include "logger.h"
#include "helper/shared.h"

class Config
{
public:
	explicit Config(const up::shared_ptr<Logger> &logger);

	enum class OptionType
	{
		Boolean,
		String,
		Integer,
	};
	typedef boost::variant<bool, int, std::string>  VariantType;

	bool addOption(const std::string &title, Config::OptionType type, const VariantType &defaultValue);
	bool addOption(const std::string &title, Config::OptionType type, const char* defaultValue);
	bool addOption(const std::string &title, Config::OptionType type, const bool &defaultValue);
	bool addOption(const std::string &title, Config::OptionType type, const int &defaultValue);
	bool addOption(const std::string &title, Config::OptionType type, const std::string &defaultValue);
	bool deleteOption(const std::string &title);

	std::pair<OptionType, VariantType> getOption(const std::string &title) const;
	bool setOption(const std::string &title, const VariantType &defaultValue);
	bool setOption(const std::string &title, const char* defaultValue);
	bool setOption(const std::string &title, const bool &defaultValue);
	bool setOption(const std::string &title, const int &defaultValue);
	bool setOption(const std::string &title, const std::string &defaultValue);

	void loadConfig(const std::string &path);
	void resetOptionsToDefaults();
	bool readLine(const std::string &lineRef);
	up::shared_ptr<Logger> getLogger() const;
	up::shared_ptr<Logger> &getLoggerRef();
	void setLogger(const up::shared_ptr<Logger> &logger);

private:
	bool checkVariant(const OptionType type, const VariantType &variant) const;
	bool addOptionVariant(const std::string &title, OptionType type, const VariantType &defaultValue);
	up::shared_ptr<Logger> logger_;
	std::unordered_map<std::string, std::pair<Config::OptionType, VariantType>> options_;
	std::unordered_map<std::string, VariantType> defaultOptions_;

	bool configWasLoadedOnce_;
};

#endif // CONFIG_H
