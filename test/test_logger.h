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

#ifndef TESTLOGGER_H
#define TESTLOGGER_H

#include <cxxtest/TestSuite.h>
#include "../src/logger.h"

class TestLogger : public CxxTest::TestSuite
{
public:
	void TestLoggerStuff()
	{
		Logger logger(Logger::OutputType::Both);
		short wasReceived = 0;
		logger.setConsoleFunction([&wasReceived](const std::string &str)
		{
			TS_ASSERT(str.find("TestString") != std::string::npos);
			++wasReceived;
		});

		logger.setFileFunction([&wasReceived](const std::string &str)
		{
			TS_ASSERT(str.find("TestString") != std::string::npos);
			++wasReceived;
		});
#define categoryFromInt(i) static_cast<Logger::CategoryType>(i)
#define outputFromInt(i) static_cast<Logger::OutputType>(i)
#define enumToSize_T(i) static_cast<std::size_t>(i)

		for(int x = enumToSize_T(Logger::OutputType::NoOutput);
				x<=enumToSize_T(Logger::OutputType::Both);
				++x)
		{
			wasReceived = 0;
			auto output = outputFromInt(x);
			logger.setOutputType(output);
			auto iMax = enumToSize_T(Logger::CategoryType::CriticalError);
			for(int i = enumToSize_T(Logger::CategoryType::Info);
					i<=iMax;
					++i)
			{
				auto category = categoryFromInt(i);
				logger.setCategoryDisplayed(category, true);
				logger.log(category, "TestString");
			}
			switch(output)
			{
				case Logger::OutputType::NoOutput:
					TS_ASSERT_EQUALS(wasReceived, 0);
					break;
				case Logger::OutputType::Console:
				case Logger::OutputType::File:
					TS_ASSERT_EQUALS(wasReceived, iMax);
					break;
				case Logger::OutputType::Both:
					TS_ASSERT_EQUALS(wasReceived, iMax*2);
					break;
			}
		}


	}
};

#endif // TESTLOGGER_H
