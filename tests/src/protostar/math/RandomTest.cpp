///
/// RandomTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>
#include <protostar/math/Random.hpp>

TEST(Random, generate)
{
	constexpr int a = 0;
	constexpr int b = 10;

	auto resultA = pr::random(a, b);
	auto resultB = pr::random(a, b);
	auto resultC = pr::random(a, b);
	auto resultD = pr::random(a, b);

	EXPECT_TRUE(resultA <= 10);
	EXPECT_TRUE(resultA >= 0);

	EXPECT_TRUE(resultB <= 10);
	EXPECT_TRUE(resultB >= 0);

	EXPECT_TRUE(resultC <= 10);
	EXPECT_TRUE(resultC >= 0);

	EXPECT_TRUE(resultD <= 10);
	EXPECT_TRUE(resultD >= 0);
}