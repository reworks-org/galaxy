///
/// TimeTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>
#include <protostar/system/Time.hpp>

TEST(Time, secondsToMilliseconds)
{
	constexpr auto ms = pr::secondsToMilliseconds(10);
	EXPECT_EQ(ms, 10000);
}

TEST(Time, secondsToMicroseconds)
{
	constexpr auto ns = pr::secondsToMicroseconds(1);
	EXPECT_EQ(ns, 1000000);
}

TEST(Time, millisecondsToSeconds)
{
	constexpr auto s = pr::millisecondsToSeconds(1000);
	EXPECT_EQ(s, 1);
}

TEST(Time, microsecondsToSeconds)
{
	constexpr auto s = pr::microsecondsToSeconds(1000000);
	EXPECT_EQ(s, 1);
}

TEST(Time, microsecondsToMilliseconds)
{
	constexpr auto ms = pr::microsecondsToMilliseconds(1000000);
	EXPECT_EQ(ms, 1000);
}