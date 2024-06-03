///
/// StringUtilsTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <iostream>

#include <gtest/gtest.h>

#include <galaxy/utils/StringUtils.hpp>

TEST(Utils, StringSplit)
{
	std::string input = "a.b";
	const auto out    = galaxy::strutils::split(input, ".");

	ASSERT_TRUE(out.size() == 2);

	EXPECT_EQ(out[0], "a");
	EXPECT_EQ(out[1], "b");
}

TEST(Utils, ReplaceFirst)
{
	std::string test = "123.456";

	galaxy::strutils::replace_first(test, "123", "456");
	EXPECT_EQ(test, "456.456");

	galaxy::strutils::replace_first(test, "456", "123");
	EXPECT_EQ(test, "123.456");

	galaxy::strutils::replace_first(test, "456", "123");
	EXPECT_EQ(test, "123.123");
}

TEST(Utils, ReplaceAll)
{
	std::string test = "123.456.123";

	galaxy::strutils::replace_all(test, "123", "456");
	EXPECT_EQ(test, "456.456.456");

	galaxy::strutils::replace_all(test, "456", "123");
	EXPECT_EQ(test, "123.123.123");
}

TEST(Utils, BeginsWith)
{
	std::string test = "testFooBar";

	EXPECT_TRUE(galaxy::strutils::begins_with(test, "test"));
	EXPECT_FALSE(galaxy::strutils::begins_with(test, "Bar"));
}

TEST(Utils, RTrim)
{
	std::string test = "  test    ";

	galaxy::strutils::rtrim(test);
	EXPECT_EQ(test, "test    ");
}

TEST(Utils, LTrim)
{
	std::string test = "  test    ";

	galaxy::strutils::rtrim(test);
	EXPECT_EQ(test, "  test");
}

TEST(Utils, Trim)
{
	std::string test = "  test    ";

	galaxy::strutils::rtrim(test);
	EXPECT_EQ(test, "test");
}

TEST(Utils, MakeSingleSpaced)
{
	std::string test = "t  e  s  t";

	galaxy::strutils::make_single_spaced(test);
	EXPECT_EQ(test, "t e s t");
}