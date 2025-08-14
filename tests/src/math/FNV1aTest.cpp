///
/// FNV1aTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>

#include <galaxy/math/FNV1a.hpp>

TEST(Math, FNV1a32)
{
	auto       input = "test";
	const auto a     = galaxy::math::fnv1a_32(input);
	const auto b     = galaxy::math::fnv1a_32(input);

	ASSERT_EQ(a, b);
}

TEST(Math, FNV1a64)
{
	auto       input = "test";
	const auto a     = galaxy::math::fnv1a_64(input);
	const auto b     = galaxy::math::fnv1a_64(input);

	ASSERT_EQ(a, b);
}

TEST(Math, FNV1a3264Different)
{
	auto       input = "test";
	const auto a     = galaxy::math::fnv1a_32(input);

	input        = "test2";
	const auto b = galaxy::math::fnv1a_64(input);

	ASSERT_NE(a, b);
}
