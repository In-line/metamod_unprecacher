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

#ifndef TEST_UNPRECACHEOPTIONS_H
#define TEST_UNPRECACHEOPTIONS_H
#include <cxxtest/TestSuite.h>

#include "../src/unprecacheoptions.h"


class TestUnprecacheOptions : public CxxTest::TestSuite
{
public:
	void Test_analyzeBitSetAlphabetPattern()
	{
		UnprecacheOptions options;

		std::string testPattern = "abc  ";
		TS_ASSERT_THROWS_NOTHING(UnprecacheOptions::stringPatternToOptions(testPattern));

		options = UnprecacheOptions::stringPatternToOptions(testPattern);

		TS_ASSERT_EQUALS(options.isNotDefault(), true);
		TS_ASSERT_EQUALS(options.deleteEntity(), true);
		TS_ASSERT_EQUALS(options.notDeleteHuman(), true);
		TS_ASSERT_EQUALS(options.replace(), true);
		TS_ASSERT_THROWS_ANYTHING(UnprecacheOptions::stringPatternToOptions("jlgke3$%%^#@@!@#$%  "));

		options = UnprecacheOptions::stringPatternToOptions("  ");

		TS_ASSERT_EQUALS(options.isNotDefault(), false);
		TS_ASSERT_EQUALS(options.deleteEntity(), false);
		TS_ASSERT_EQUALS(options.notDeleteHuman(), false);
		TS_ASSERT_EQUALS(options.replace(), false);
	}
	void Test_EqualityOperator()
	{
		UnprecacheOptions a,b;
		TS_ASSERT(a==b);
		a.setDeleteEntity(true);
		TS_ASSERT(a!=b);
		a.setDeleteEntity(false);
		b.setReplacedPath("Test");
		TS_ASSERT(a!=b);
	}
};

#endif // TEST_UNPRECACHEOPTIONS_H
