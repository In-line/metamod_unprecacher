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

#ifndef TESTSTRING_UTILS_H
#define TESTSTRING_UTILS_H

#include <cxxtest/TestSuite.h>

#include "../src/helper/string_utils.h"
#include <iomanip>
class TestStringUtils : public CxxTest::TestSuite
{
public:
	void Test_parseString()
	{
		std::vector<std::string> result = parseString("models/w_usp.mdl", true);
		TS_ASSERT_EQUALS(result.size(), 1);
		TS_ASSERT_EQUALS(result[0], "models/w_usp.mdl");

		for(short i = 0; i<2;++i)
		for(short b = 0; b<2;++b)
		{
			std::vector<std::string> result =
					parseString(" STRING1 STRING2 STRING3\t\t\t \"STRING4_ \tQUOTE\" STRING5    ", bool(i), bool(b));
			TS_ASSERT_EQUALS(result.size(),5);
			if(result.size() == 5)
			{
				TS_ASSERT_EQUALS(result[0],"STRING1");
				TS_ASSERT_EQUALS(result[1],"STRING2");
				if(b == 0)
				{
					TS_ASSERT_EQUALS(result[2],"STRING3\t\t\t");
				}
				else
				{
					TS_ASSERT_EQUALS(result[2],"STRING3");
				}

				if(i == 0)
				{
					TS_ASSERT_EQUALS(result[3],"\"STRING4_ \tQUOTE\"");
				}
				else
				{
					TS_ASSERT_EQUALS(result[3],"STRING4_ \tQUOTE");
				}

				TS_ASSERT_EQUALS(result[4],"STRING5");
			}
		}
	}

	void Test_is_string_whitespaces()
	{
			TS_ASSERT_EQUALS(is_string_whitespaces("   \t     \t\t\t\t\t"),true);
			TS_ASSERT_EQUALS(is_string_whitespaces(" NF  \t     \t\t\t\t\t"),false);
			TS_ASSERT_EQUALS(is_string_whitespaces(" N  \t     \t\t\t\t\tN "),false);
	}

	void Test_starts_with()
	{
			TS_ASSERT_EQUALS(starts_with("ABC", "AB"), true);
			TS_ASSERT_EQUALS(starts_with("ABC", "LONGER_THEN_ABC"), false);
			TS_ASSERT_EQUALS(starts_with("32323121", "ABC"), false);
			TS_ASSERT_EQUALS(starts_with("  32323121", " "), true);
			TS_ASSERT_EQUALS(starts_with("", ""), false);
	}

	void Test_removeComments()
	{
		TS_ASSERT_EQUALS(removeComments("Text // Comment"), "Text ");
		TS_ASSERT_EQUALS(removeComments("Text "), "Text ");
		TS_ASSERT_EQUALS(removeComments("models/w_knife.mdl c models/v_knife_custom.mdl"), "models/w_knife.mdl c models/v_knife_custom.mdl");

	}

	void Test_trim()
	{
		TS_ASSERT_EQUALS(trim("   Text  "), "Text");
		TS_ASSERT_EQUALS(trim("   Text"), "Text");
		TS_ASSERT_EQUALS(trim("Text   "), "Text");

		TS_ASSERT_EQUALS(trim("models/w_knife.mdl c models/v_knife_custom.mdl"), "models/w_knife.mdl c models/v_knife_custom.mdl");

		TS_ASSERT_EQUALS(trim("  "), "");
	}

	void Test_str_put_time()
	{
		std::time_t curTime = std::time(nullptr);
		auto localTime = std::localtime(&curTime);

		auto convert = [](const std::tm* tmb, const char* format) -> std::string
		{
			std::stringstream ss;
			ss << std::put_time(tmb, format);
			return ss.str();
		};
		TS_ASSERT_EQUALS(str_put_time(localTime, "%M%D%S%Y"), convert(localTime, "%M%D%S%Y"));
	}
};

#endif // TESTSTRING_UTILS_H
