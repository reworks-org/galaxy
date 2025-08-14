///
/// MathTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>

#include <galaxy/math/Math.hpp>

TEST(Math, NormalizeLow)
{
	constexpr auto result = galaxy::math::normalize(0, 100);
	EXPECT_EQ(result, 0.0f);
}

TEST(Math, NormalizeMed)
{
	constexpr auto result = galaxy::math::normalize(50, 100);
	EXPECT_EQ(result, 0.5f);
}

TEST(Math, NormalizeHigh)
{
	constexpr auto result = galaxy::math::normalize(255, 255);
	EXPECT_EQ(result, 1.0f);
}
