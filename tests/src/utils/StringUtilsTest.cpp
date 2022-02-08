///
/// StringUtilsTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

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
	unsigned int input = 00000022;

	const auto out = galaxy::strutils::parse_codepoint(input);
	EXPECT_EQ(out, "!");
}