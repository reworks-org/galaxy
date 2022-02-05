///
/// ZLibTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>

#include <galaxy/algorithm/ZLib.hpp>

TEST(Algorithm, ZLib)
{
	auto input = "Test";

	auto encoded = galaxy::algorithm::encode_zlib(input);
	auto decoded = galaxy::algorithm::decode_zlib(encoded);

	ASSERT_EQ(decoded, input);

	auto encoded_twice = galaxy::algorithm::encode_zlib(decoded);
	EXPECT_EQ(encoded, encoded_twice);
}