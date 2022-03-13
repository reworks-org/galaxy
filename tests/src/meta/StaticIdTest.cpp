///
/// StaticIdTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>

#include <galaxy/meta/StaticId.hpp>

using TestUID1 = galaxy::meta::StaticId<struct Test_>;
using TestUID2 = galaxy::meta::StaticId<struct _Test>;

struct TestA
{
	TestA() = default;
};

struct TestB
{
	TestB() = default;
};

TEST(StaticId, Get)
{
	const auto a = TestUID1::get();
	const auto b = TestUID1::get();

	const auto a2 = TestUID2::get();
	const auto b2 = TestUID2::get();

	EXPECT_EQ(a, 0);
	EXPECT_EQ(b, 1);

	EXPECT_EQ(a2, 0);
	EXPECT_EQ(b2, 1);
}