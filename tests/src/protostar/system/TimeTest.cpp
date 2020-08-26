///
/// TimeTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>
#include <protostar/system/Time.hpp>

TEST(Time, seconds_to_milliseconds)
{
	auto ms = pr::seconds_to_milliseconds(10);
	EXPECT_EQ(ms, 10000);
}

TEST(Time, seconds_to_microseconds)
{
	auto ns = pr::seconds_to_microseconds(1);
	EXPECT_EQ(ns, 1000000);
}

TEST(Time, milliseconds_to_seconds)
{
	auto s = pr::milliseconds_to_seconds(1000);
	EXPECT_EQ(s, 1);
}

TEST(Time, microseconds_to_seconds)
{
	auto s = pr::microseconds_to_seconds(1000000);
	EXPECT_EQ(s, 1);
}

TEST(Time, microseconds_to_milliseconds)
{
	auto ms = pr::microseconds_to_milliseconds(1000000);
	EXPECT_EQ(ms, 1000);
}