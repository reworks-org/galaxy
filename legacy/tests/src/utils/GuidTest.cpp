///
/// GuidTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>

#include <galaxy/utils/Guid.hpp>

TEST(Core, Guid)
{
	galaxy::utils::Guid a;
	galaxy::utils::Guid b;

	ASSERT_TRUE(!a.is_empty());
	ASSERT_TRUE(!b.is_empty());

	EXPECT_FALSE(a.to_string().empty());
	EXPECT_FALSE(b.to_string().empty());

	EXPECT_TRUE(a != b);
}