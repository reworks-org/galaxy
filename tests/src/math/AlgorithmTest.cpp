///
/// AlgorithmTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>

#include <galaxy/math/Algorithms.hpp>

TEST(Math, Normalize)
{
	constexpr auto max      = 100;
	constexpr auto value    = 50;
	constexpr auto expected = 0.5f;

	auto result = galaxy::math::normalize(value, max);
	EXPECT_EQ(expected, result);
}

TEST(Math, VectorContains)
{
	constexpr auto value          = 42;
	const std::vector<int> values = {20, 42, 100};
	const std::vector<int> empty  = {};

	const auto result1 = galaxy::math::contains(values, value);
	EXPECT_TRUE(result1 == value);

	const auto result2 = galaxy::math::contains(empty, value);
	EXPECT_TRUE(result2 == false);
}