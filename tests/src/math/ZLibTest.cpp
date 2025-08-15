///
/// ZLibTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>

#include <galaxy/math/ZLib.hpp>

TEST(Math, ZLib)
{
	auto input = "Test";

	auto encoded = galaxy::ZLib::encode(input);
	auto decoded = galaxy::ZLib::decode(encoded);

	ASSERT_EQ(decoded, input);

	auto encoded_twice = galaxy::ZLib::encode(decoded);
	EXPECT_EQ(encoded, encoded_twice);
}
