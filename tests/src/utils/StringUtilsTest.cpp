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

	EXPECT_EQ(galaxy::strutils::replace_first(test, "123", "456"), "456.456");
	EXPECT_EQ(galaxy::strutils::replace_first(test, "456", "123"), "123.456");
	EXPECT_EQ(galaxy::strutils::replace_first(test, "456", "123"), "123.123");
}

TEST(Utils, ReplaceAll)
{
	std::string test = "123.456.123";
	EXPECT_EQ(galaxy::strutils::replace_all(test, "123", "456"), "456.456.456");
	EXPECT_EQ(galaxy::strutils::replace_all(test, "456", "123"), "123.123.123");
}