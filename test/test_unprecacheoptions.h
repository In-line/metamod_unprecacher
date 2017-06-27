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
		TS_ASSERT_THROWS_NOTHING(UnprecacheOptions::analyzeBitSetAlphabetPattern(testPattern));

		options = UnprecacheOptions::analyzeBitSetAlphabetPattern(testPattern);

		TS_ASSERT_EQUALS(options.isNotDefault(), true);
		TS_ASSERT_EQUALS(options.deleteEntity(), true);
		TS_ASSERT_EQUALS(options.notDeleteHuman(), true);
		TS_ASSERT_EQUALS(options.replace(), true);
		TS_ASSERT_THROWS_ANYTHING(UnprecacheOptions::analyzeBitSetAlphabetPattern("jlgke3$%%^#@@!@#$%  "));

		options = UnprecacheOptions::analyzeBitSetAlphabetPattern("  ");

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
