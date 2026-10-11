///
/// main.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>

#include <galaxy/platform/Pragma.hpp>

TEST(Platform, Detect)
{
#ifdef GALAXY_WIN_PLATFORM
	ASSERT_TRUE(true);
#else
	ASSERT_TRUE(false);
#endif
}
