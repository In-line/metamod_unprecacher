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

#ifndef TEST_CONFIG_H
#define TEST_CONFIG_H

#include <cxxtest/TestSuite.h>
#include "../src/config.h"

class TestConfig : public CxxTest::TestSuite
{
private:
	std::shared_ptr<Logger> createLogger()
	{
		auto logger = std::make_shared<Logger>();
		logger->setCategoryDisplayed(Logger::CategoryType::CriticalError, true);
		logger->setCategoryDisplayed(Logger::CategoryType::Error, true);
		logger->setCategoryDisplayed(Logger::CategoryType::Warning, true);
		logger->setOutputType(Logger::OutputType::Console);
		logger->setConsoleFunction([](const std::string &str)
		{
			TS_WARN(str);
			TS_ASSERT(false);
		});
		return logger;
	}

public:
	void Test_readLine()
	{
		Config config(createLogger());
		//config.addOption("test_option_bool", Config::OptionType::Boolean, true);
		TS_ASSERT(config.addOption("test_option_bool", Config::OptionType::Boolean, false));
		TS_ASSERT(config.addOption("test_option_int", Config::OptionType::Integer, 0));
		TS_ASSERT(config.addOption("test_option_string", Config::OptionType::String, "Default"));

		config.readLine("test_option_bool =     yes");
		config.readLine("test_option_int =     13");
		config.readLine("test_option_string = NotDefault");

		TS_ASSERT_EQUALS(config.getOption("test_option_bool").first,Config::OptionType::Boolean);
		TS_ASSERT_EQUALS(boost::get<bool>(config.getOption("test_option_bool").second),true);

		TS_ASSERT_EQUALS(config.getOption("test_option_string").first,Config::OptionType::String);
		TS_ASSERT_EQUALS(boost::get<std::string>(config.getOption("test_option_string").second), "NotDefault");


		TS_ASSERT_EQUALS(config.getOption("test_option_int").first,Config::OptionType::Integer);
		TS_ASSERT_EQUALS(boost::get<int>(config.getOption("test_option_int").second), 13);

		config.resetOptionsToDefaults();

		TS_ASSERT_EQUALS(boost::get<bool>(config.getOption("test_option_bool").second), false);
		TS_ASSERT_EQUALS(boost::get<std::string>(config.getOption("test_option_string").second), "Default");
		TS_ASSERT_EQUALS(boost::get<int>(config.getOption("test_option_int").second), 0);
	}

	void test_setOption()
	{
		Config config(createLogger());

		TS_ASSERT(config.addOption("test_option_bool", Config::OptionType::Boolean, false));
		TS_ASSERT(config.addOption("test_option_int", Config::OptionType::Integer, 0));
		TS_ASSERT(config.addOption("test_option_string", Config::OptionType::String, "Default"));

		TS_ASSERT(config.setOption("test_option_bool", false));
		TS_ASSERT_EQUALS(boost::get<bool>(config.getOption("test_option_bool").second), false);

		TS_ASSERT(config.setOption("test_option_string", "MbDefault"));
		TS_ASSERT_EQUALS(boost::get<std::string>(config.getOption("test_option_string").second), "MbDefault");

		TS_ASSERT(config.setOption("test_option_int", 5));
		TS_ASSERT_EQUALS(boost::get<int>(config.getOption("test_option_int").second), 5);

		TS_ASSERT(!config.setOption("test_option_string23", "MbDefault"));
		TS_ASSERT(!config.setOption("test", "MbDefault"));
	}

	void test_resetOptionsToDefaults()
	{
			Config config(createLogger());

			TS_ASSERT(config.addOption("test_option_bool", Config::OptionType::Boolean, false));
			TS_ASSERT(config.addOption("test_option_int", Config::OptionType::Integer, 0));
			TS_ASSERT(config.addOption("test_option_string", Config::OptionType::String, "Default"));

			TS_ASSERT(config.setOption("test_option_bool", true));
			TS_ASSERT(config.setOption("test_option_string", "MbDefault"));
			TS_ASSERT(config.setOption("test_option_int", 5));

			config.resetOptionsToDefaults();

			TS_ASSERT_EQUALS(boost::get<bool>(config.getOption("test_option_bool").second),false);
			TS_ASSERT_EQUALS(boost::get<std::string>(config.getOption("test_option_string").second), "Default");
			TS_ASSERT_EQUALS(boost::get<int>(config.getOption("test_option_int").second), 0);
	}

	void test_deleteOption()
	{
		Config config(createLogger());
		TS_ASSERT(config.addOption("test_option_bool", Config::OptionType::Boolean, false));
		TS_ASSERT(!config.addOption("test_option_bool", Config::OptionType::Boolean, false));

		TS_ASSERT(!config.deleteOption("test_option_mb1234"));

		TS_ASSERT(config.deleteOption("test_option_bool"));
		TS_ASSERT_THROWS_ANYTHING(config.getOption("test_option_bool"));

		TS_ASSERT(!config.deleteOption("test_option_bool"));

	}
};

#endif // TEST_CONFIG_H
