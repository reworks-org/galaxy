///
/// Base64Test.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>

#include <galaxy/math/Base64.hpp>

TEST(Math, Base64)
{
	auto input = "test";

	auto encoded = galaxy::math::encode_base64(input);
	auto decoded = galaxy::math::decode_base64(encoded);

	ASSERT_EQ(decoded, input);

	auto encoded_twice = galaxy::math::encode_base64(decoded);
	EXPECT_EQ(encoded, encoded_twice);
}
