///
/// Config.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <type_traits>

#include <gtest/gtest.h>
#include <solar/Config.hpp>

TEST(Config, DefaultSRInteger)
{
	constexpr bool result = std::is_same<SR_INTEGER, std::uint32_t>::value;
	ASSERT_TRUE(result);
}

TEST(Config, DefaultEntityType)
{
	constexpr bool result = std::is_same<sr::Entity, SR_INTEGER>::value;
	ASSERT_TRUE(result);
}

TEST(Config, ValidEntityIdentifier)
{
	ASSERT_EQ(sr::VALID_ENTITY, 0x01);
}