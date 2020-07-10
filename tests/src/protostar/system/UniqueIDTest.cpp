///
/// UniqueIDTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>
#include <protostar/system/UniqueID.hpp>

using TestUID = protostar::UniqueID<struct TestSpec_>;

class TestA {};
class TestB {};

TEST(UniqueID, Get)
{
	const auto a = TestUID::get<TestA>();
	const auto b = TestUID::get<TestB>();

	const auto a2 = TestUID::get<TestA>();
	const auto b2 = TestUID::get<TestB>();

	EXPECT_EQ(a, 0);
	EXPECT_EQ(b, 0);

	EXPECT_EQ(a2, 1);
	EXPECT_EQ(b2, 1);
}