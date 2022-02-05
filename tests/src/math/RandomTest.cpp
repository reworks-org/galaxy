///
/// DispatcherTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>

#include <galaxy/math/Random.hpp>

TEST(Math, RandomLargeMinMax)
{
	constexpr auto min = 10;
	constexpr auto max = 20;

	const auto result = galaxy::math::random(min, max);
	EXPECT_TRUE(result >= 10 && result <= 20);
}

TEST(Math, RandomSmallMinMax)
{
	constexpr auto min = 10;
	constexpr auto max = 12;

	const auto result = galaxy::math::random(min, max);
	EXPECT_TRUE(result >= 10 && result <= 12);
}

TEST(Math, RandomTinyMinMax)
{
	constexpr auto min = 10;
	constexpr auto max = 11;

	const auto result = galaxy::math::random(min, max);
	EXPECT_TRUE(result >= 10 && result <= 11);
}

TEST(Math, RandomSameMinMax)
{
	constexpr auto min = 10;
	constexpr auto max = 10;

	const auto result = galaxy::math::random(min, max);
	EXPECT_TRUE(result == 10);
}