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
		module.revalidateEnds();
		UnprecacheOptions o = UnprecacheOptions::analyzeBitSetAlphabitePattern("abc");
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

	void test_analyzeLoggerVerbosityString()
	{
		std::size_t categoryStart = static_cast<std::size_t>(Logger::CategoryType::Debug);
		std::size_t categoryMax = static_cast<std::size_t>(Logger::CategoryType::CriticalError);

		Module module;

		std::string verbString;
		for(char i = char(categoryStart); i<=char(categoryMax);++i)
		{
			verbString += 'a' + char(i);
		}
		module.analyzeLoggerVerbosityString(verbString);

		for(std::size_t i = categoryStart; i<=categoryMax;++i)
		{
			TS_ASSERT(module.getLoggerRef()->getCategoryDisplayed(i));
		}
	}
};

#endif // TEST_MODULE_H
