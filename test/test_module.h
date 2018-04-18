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


#ifndef TEST_MODULE_H
#define TEST_MODULE_H


#include <cxxtest/TestSuite.h>
#include "../src/module.h"
class TestModule : public CxxTest::TestSuite
{
public:
	void test_readLine()
	{
		Module module;
		module.getLoggerRef()->setCategoryDisplayed(Logger::CategoryType::Error, true);
		module.getLoggerRef()->setCategoryDisplayed(Logger::CategoryType::CriticalError, true);
		module.getLoggerRef()->setOutputType(Logger::OutputType::Console);
		module.getLoggerRef()->setConsoleFunction([&](const std::string &inputStr)
		{
			TS_WARN(inputStr);
			TS_ASSERT(false);
		});

		TS_ASSERT(module.readLine("models/w_usp.mdl abc models/v_knife_custom.something"));
		TS_ASSERT(module.readLine("sprites/w_usp.spr abc models/v_knife_custom.something"));
		TS_ASSERT(module.readLine("sound/w_usp.wav abc models/v_knife_custom.something"));
		UnprecacheOptions o = UnprecacheOptions::stringPatternToOptions("abc");
		o.setReplacedPath("models/v_knife_custom.something");

		TS_ASSERT(module.checkModel("models/w_usp.mdl"));
		TS_ASSERT_EQUALS(module.getLastHitPoint(), o);

		TS_ASSERT(module.checkSprite("sprites/w_usp.spr"));
		TS_ASSERT_EQUALS(module.getLastHitPoint(), o);

		TS_ASSERT(module.checkSound("w_usp.wav"));
		TS_ASSERT_EQUALS(module.getLastHitPoint(), o);

		TS_ASSERT(!module.checkModel("test1"));
		TS_ASSERT(!module.checkSprite("test2"));
		TS_ASSERT(!module.checkSound("test3"));

		module.getLoggerRef()->setOutputType(Logger::OutputType::NoOutput);

		TS_ASSERT(!module.readLine("test"));
	}

	void test_analyzeLoggerStringPattern()
	{
		std::size_t categoryStart = static_cast<std::size_t>(Logger::CategoryType::Debug);
		std::size_t categoryMax = static_cast<std::size_t>(Logger::CategoryType::CriticalError);

		Module module;

		std::string verbString;
		for(char i = char(categoryStart); i<=char(categoryMax);++i)
		{
			verbString += 'a' + i;
		}
		module.analyzeLoggerStringPattern(verbString);

		for(std::size_t i = categoryStart; i<=categoryMax;++i)
		{
			TS_ASSERT(module.getLoggerRef()->getCategoryDisplayed(i));
		}
	}
};

#endif // TEST_MODULE_H
