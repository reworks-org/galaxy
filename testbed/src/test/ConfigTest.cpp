///
/// ConfigTest.cpp
///
/// solar
/// See LICENSE.txt.
///

#include <type_traits>

#include <solar/Config.hpp>

#include "gtest/gtest.h"

TEST(Config, DefaultValues)
{
	#ifdef SIXTYFOURBIT
		ASSERT_EQ(std::is_same<SR_INTEGER, std::uint64_t>::value, true);
	#else
		ASSERT_EQ(std::is_same<SR_INTEGER, std::uint32_t>::value, true);
	#endif

	
	ASSERT_EQ(std::is_same<sr::Entity, SR_INTEGER>::value, true);
}