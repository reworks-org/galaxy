///
/// UniqueIDTest.cpp
///
/// solar
/// See LICENSE.txt.
///

#include <protostar/utility/UniqueID.hpp>

#include "gtest/gtest.h"

///
/// Define unique types for testing.
///
using testUniqueIDA = protostar::UniqueID<struct TestUniqueIDA>;
using testUniqueIDB = protostar::UniqueID<struct TestUniqueIDB>;

TEST(UniqueID, Functionality)
{
	// Each type should have a unique ID starting from 0 for that specialization.
	auto UniqueID1 = testUniqueIDA::get<int>();
	auto UniqueID2 = testUniqueIDA::get<bool>();
	auto UniqueID3 = testUniqueIDA::get<float>();

	ASSERT_EQ(UniqueID1, 0);
	ASSERT_EQ(UniqueID2, 1);
	ASSERT_EQ(UniqueID3, 2);

	// B type should also start from 0 not continue on and should have a unique id for each type.	 
	auto UniqueID1b = testUniqueIDB::get<short>();
	auto UniqueID2b = testUniqueIDB::get<long>();
	auto UniqueID3b = testUniqueIDB::get<char>();

	ASSERT_EQ(UniqueID1b, 0);
	ASSERT_EQ(UniqueID2b, 1);
	ASSERT_EQ(UniqueID3b, 2);
}