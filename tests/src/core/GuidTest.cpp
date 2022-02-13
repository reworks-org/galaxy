///
/// GuidTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>

#include <galaxy/core/Guid.hpp>

TEST(Core, Guid)
{
	galaxy::core::Guid a;
	galaxy::core::Guid b;

	ASSERT_TRUE(!a.is_empty());
	ASSERT_TRUE(!b.is_empty());

	EXPECT_FALSE(a.to_string().empty());
	EXPECT_FALSE(b.to_string().empty());

	EXPECT_TRUE(a != b);
}