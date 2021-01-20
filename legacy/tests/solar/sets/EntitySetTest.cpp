///
/// EntitySetTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>
#include <solar/sets/EntitySet.hpp>

///
/// Tests if has() correctly returns false on an empty set.
///
TEST(EntitySet, EmptySetHas)
{
	// In empty set.
	sr::EntitySet<sr::Entity> set;
	constexpr int a = 1;

	ASSERT_FALSE(set.has(a));
}

///
/// Tests that after inserting an element, has() returns true that the element was inserted.
///
TEST(EntitySet, InsertHas)
{
	sr::EntitySet<sr::Entity> set;

	// Insert single uniques.
	constexpr int a = 1;
	constexpr int b = 2;
	constexpr int c = 3;

	set.insert(a);
	set.insert(b);
	set.insert(c);

	EXPECT_EQ(set.has(a), true);
	EXPECT_EQ(set.has(b), true);
	EXPECT_EQ(set.has(c), true);

	EXPECT_EQ(set.has(10), false);
}

///
/// Test that empty set has a size of 0.
///
TEST(EntitySet, EmptySetSize)
{
	sr::EntitySet<sr::Entity> set;
	ASSERT_EQ(set.size(), 0);
}

///
/// Tests that size is correctly updated.
///
TEST(EntitySet, InsertSize)
{
	sr::EntitySet<sr::Entity> set;
	constexpr int a = 1;
	constexpr int b = 2;

	set.insert(a);
	set.insert(b);

	EXPECT_EQ(set.size(), 2);
}

///
/// Tests capacity of an empty set.
///
TEST(EntitySet, EmptySetCapacity)
{
	sr::EntitySet<sr::Entity> set;
	ASSERT_EQ(set.capacity(), 0);
}

///
/// Tests that capacity is correctly updated.
///
TEST(EntitySet, InsertCapacity)
{
	sr::EntitySet<sr::Entity> set;
	constexpr int a = 1;
	constexpr int b = 2;

	set.insert(a);
	set.insert(b);

	// 2 inserts + 1 = 3.
	EXPECT_EQ(set.capacity(), 3);
}

///
/// Test a set is empty with no insertions.
///
TEST(EntitySet, Empty)
{
	sr::EntitySet<sr::Entity> set;

	ASSERT_TRUE(set.empty());
}

///
/// Test to make sure a set is not empty after insertion.
///
TEST(EntitySet, NotEmpty)
{
	sr::EntitySet<sr::Entity> set;
	constexpr int a = 1;
	constexpr int b = 2;
	constexpr int c = 3;
	constexpr int d = 4;

	set.insert(a);
	set.insert(b);
	set.insert(c);
	set.insert(d);

	ASSERT_FALSE(set.empty());
}

///
/// Test to ensure an element is removed with has.
///
TEST(EntitySet, RemoveHas)
{
	// Perform setup.
	sr::EntitySet<sr::Entity> set;
	constexpr int a = 1;
	constexpr int b = 2;
	constexpr int c = 3;
	constexpr int d = 4;

	set.insert(a);
	set.insert(b);
	set.insert(c);
	set.insert(d);

	// Remove and validate.
	set.remove(a);
	ASSERT_EQ(set.has(a), false);
	ASSERT_EQ(set.has(b), true);
	ASSERT_EQ(set.has(c), true);
	ASSERT_EQ(set.has(d), true);
}

///
/// Test to ensure an element is removed with size.
///
TEST(EntitySet, RemoveSize)
{
	// Perform setup.
	sr::EntitySet<sr::Entity> set;
	constexpr int a = 1;
	constexpr int b = 2;
	constexpr int c = 3;
	constexpr int d = 4;

	set.insert(a);
	set.insert(b);
	set.insert(c);
	set.insert(d);

	// Remove and validate.
	set.remove(d);
	ASSERT_EQ(set.size(), 3);
}

///
/// Test to ensure an element is removed with capacity.
///
TEST(EntitySet, RemoveCapacity)
{
	// Perform setup.
	sr::EntitySet<sr::Entity> set;
	constexpr int a = 1;
	constexpr int b = 2;
	constexpr int c = 3;
	constexpr int d = 4;

	set.insert(a);
	set.insert(b);
	set.insert(c);
	set.insert(d);

	// Remove and validate.
	set.remove(c);

	// Capacity should not shrink.
	// 4 inserts + 1 = 5.
	ASSERT_EQ(set.capacity(), 5);
}

///
/// Test a set is empty after removal of all insertions.
///
TEST(EntitySet, EmptyAfterRemove)
{
	// Perform setup.
	sr::EntitySet<sr::Entity> set;
	constexpr int a = 1;

	set.insert(a);
	set.remove(a);

	ASSERT_TRUE(set.empty());
}

///
/// Test if duplicates are not inserted.
///
TEST(EntitySet, InsertDuplication)
{
	sr::EntitySet<sr::Entity> set;
	constexpr int a = 1;
	constexpr int b = 1;

	// Insert duplicate.
	set.insert(a);
	set.insert(b);

	// Duplicate should not impact size or capacity.
	EXPECT_EQ(set.size(), 1);

	// 1 insert + 1 = 2.
	EXPECT_EQ(set.capacity(), 2);
}

///
/// Test for finding index in an empty set.
///
TEST(EntitySet, EmptySetFindIndex)
{
	sr::EntitySet<sr::Entity> set;
	constexpr int a = 1;

	auto opt = set.find_index(a);
	ASSERT_EQ(opt, std::nullopt);
}

///
/// Test for finding index in a non empty set.
///
TEST(EntitySet, FindIndex)
{
	sr::EntitySet<sr::Entity> set;
	constexpr int a = 1;
	constexpr int b = 2;
	constexpr int c = 3;

	set.insert(a);
	set.insert(b);
	set.insert(c);

	auto optA = set.find_index(a);
	EXPECT_EQ(optA.value(), 0);

	auto optB = set.find_index(b);
	EXPECT_EQ(optB.value(), 1);

	auto optC = set.find_index(c);
	EXPECT_EQ(optC.value(), 2);
}

///
/// Test for finding index after removal.
///
TEST(EntitySet, FindIndexAfterRemove)
{
	sr::EntitySet<sr::Entity> set;
	constexpr int a = 1;
	constexpr int b = 2;
	constexpr int c = 3;

	set.insert(a);
	set.insert(b);
	set.insert(c);
	set.remove(b);

	auto optA = set.find_index(a);
	EXPECT_EQ(optA.value(), 0);

	auto optB = set.find_index(c);
	EXPECT_EQ(optB.value(), 1);
}