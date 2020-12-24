///
/// UniqueIDTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>
#include <galaxy/system/UniqueID.hpp>

using TestUID1 = pr::UniqueID<struct Test_>;
using TestUID2 = pr::UniqueID<struct _Test>;

class TestA
{
};

class TestB
{
};

TEST(UniqueID, get)
{
	const auto a = TestUID1::get<TestA>();
	const auto b = TestUID1::get<TestB>();

	const auto a2 = TestUID2::get<TestA>();
	const auto b2 = TestUID2::get<TestB>();

	EXPECT_EQ(a, 0);
	EXPECT_EQ(b, 1);

	EXPECT_EQ(a2, 0);
	EXPECT_EQ(b2, 1);
}