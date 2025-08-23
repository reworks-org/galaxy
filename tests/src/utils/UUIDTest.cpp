///
/// UUIDTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <regex>

#include <gtest/gtest.h>

#include <galaxy/utils/UUID.hpp>

TEST(UUID, Valid)
{
	galaxy::UUID uuid;

	std::regex regex(R"(^[0-9A-F]{8}-[0-9A-F]{4}-4[0-9A-F]{3}-[89AB][0-9A-F]{3}-[0-9A-F]{12}$)", std::regex::icase);
	ASSERT_TRUE(std::regex_match(uuid.str(), regex));
}
