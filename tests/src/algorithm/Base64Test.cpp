///
/// Base64Test.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>

#include <galaxy/algorithm/Base64.hpp>

TEST(Algorithm, Base64)
{
	auto input = "Test";

	auto encoded = galaxy::algorithm::encode_base64(input);
	auto decoded = galaxy::algorithm::decode_base64(encoded);

	ASSERT_EQ(decoded, input);

	auto encoded_twice = galaxy::algorithm::encode_base64(decoded);
	EXPECT_EQ(encoded, encoded_twice);
}