///
/// DispatcherTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>

#include <galaxy/math/Random.hpp>

TEST(Algorithm, RandomLargeMinMax)
{
	constexpr auto min = 10;
	constexpr auto max = 20;

	const auto result = galaxy::math::random(min, max);
	EXPECT_TRUE(result >= 10 && result <= 20);
}

TEST(Algorithm, RandomSmallMinMax)
{
	constexpr auto min = 10;
	constexpr auto max = 12;

	const auto result = galaxy::math::random(min, max);
	EXPECT_TRUE(result >= 10 && result <= 12);
}

TEST(Algorithm, RandomTinyMinMax)
{
	constexpr auto min = 10;
	constexpr auto max = 11;

	const auto result = galaxy::math::random(min, max);
	EXPECT_TRUE(result >= 10 && result <= 11);
}

TEST(Algorithm, RandomSameMinMax)
{
	constexpr auto min = 10;
	constexpr auto max = 10;

	const auto result = galaxy::math::random(min, max);
	EXPECT_TRUE(result == 10);
}