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

TEST(Utils, CodePointToString)
{
	EXPECT_EQ(galaxy::strutils::parse_codepoint(0x00000021), "!");
}