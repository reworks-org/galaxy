///
/// GZipTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>

#include <galaxy/math/GZip.hpp>

TEST(Math, GZip)
{
	auto input = "Test";

	auto encoded = galaxy::math::encode_gzip(input);
	auto decoded = galaxy::math::decode_gzip(encoded);

	ASSERT_EQ(decoded, input);

	auto encoded_twice = galaxy::math::encode_gzip(decoded);
	EXPECT_EQ(encoded, encoded_twice);
}