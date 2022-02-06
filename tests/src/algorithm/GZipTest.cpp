///
/// GZipTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>

#include <galaxy/algorithm/GZip.hpp>

TEST(Algorithm, GZip)
{
	auto input = "Test";

	auto encoded = galaxy::algorithm::encode_gzip(input);
	auto decoded = galaxy::algorithm::decode_gzip(encoded);

	ASSERT_EQ(decoded, input);

	auto encoded_twice = galaxy::algorithm::encode_gzip(decoded);
	EXPECT_EQ(encoded, encoded_twice);
}