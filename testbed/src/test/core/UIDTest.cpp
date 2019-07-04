///
/// UIDTest.cpp
///
/// solar
/// See LICENSE.txt.
///

#include <solar/core/UID.hpp>

#include "gtest/gtest.h"

///
/// Define unique types for testing.
///
using testUIDA = sr::UID<struct TestUniqueIDA>;
using testUIDB = sr::UID<struct TestUniqueIDB>;

TEST(UID, Functionality)
{
	// Each type should have a unique ID starting from 0 for that specialization.
	auto uid1 = testUIDA::uid<int>();
	auto uid2 = testUIDA::uid<bool>();
	auto uid3 = testUIDA::uid<float>();

	ASSERT_EQ(uid1, 0);
	ASSERT_EQ(uid2, 1);
	ASSERT_EQ(uid3, 2);

	// B type should also start from 0 not continue on and should have a unique id for each type.	 
	auto uid1b = testUIDB::uid<short>();
	auto uid2b = testUIDB::uid<long>();
	auto uid3b = testUIDB::uid<char>();

	ASSERT_EQ(uid1b, 0);
	ASSERT_EQ(uid2b, 1);
	ASSERT_EQ(uid3b, 2);
}