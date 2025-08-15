///
/// StringUtilsTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <iostream>

#include <gtest/gtest.h>

#include <galaxy/utils/StringUtils.hpp>

TEST(StrUtils, StringSplit)
{
	std::string input = "a.b";
	const auto  out   = galaxy::str::split(input, ".");

	ASSERT_TRUE(out.size() == 2);

	EXPECT_EQ(out[0], "a");
	EXPECT_EQ(out[1], "b");
}

TEST(StrUtils, ReplaceFirst)
{
	std::string test = "123.456";

	test = galaxy::str::replace_first(test, "123", "456");
	EXPECT_EQ(test, "456.456");

	test = galaxy::str::replace_first(test, "456", "123");
	EXPECT_EQ(test, "123.456");

	test = galaxy::str::replace_first(test, "456", "123");
	EXPECT_EQ(test, "123.123");
}

TEST(StrUtils, ReplaceAll)
{
	std::string test = "123.456.123";

	test = galaxy::str::replace_all(test, "123", "456");
	EXPECT_EQ(test, "456.456.456");

	test = galaxy::str::replace_all(test, "456", "123");
	EXPECT_EQ(test, "123.123.123");
}

TEST(StrUtils, BeginsWith)
{
	std::string test = "testFooBar";

	EXPECT_TRUE(galaxy::str::begins_with(test, "test"));
	EXPECT_FALSE(galaxy::str::begins_with(test, "Bar"));
}

TEST(StrUtils, LeftTrim)
{
	std::string test = "  test    ";

	test = galaxy::str::ltrim(test);
	EXPECT_EQ(test, "test    ");
}

TEST(StrUtils, RightTrim)
{
	std::string test = "  test    ";

	test = galaxy::str::rtrim(test);
	EXPECT_EQ(test, "  test");
}

TEST(StrUtils, Trim)
{
	std::string test = "  test    ";

	test = galaxy::str::trim(test);
	EXPECT_EQ(test, "test");
}

TEST(StrUtils, MakeSingleSpaced)
{
	std::string test = "t  e  s  t";

	test = galaxy::str::make_single_spaced(test);
	EXPECT_EQ(test, "t e s t");
}
