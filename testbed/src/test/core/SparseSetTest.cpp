///
/// SparseSetTest.cpp
///
/// solar
/// See LICENSE.txt.
///

#include <solar/core/SparseSet.hpp>

#include "gtest/gtest.h"

TEST(SparseSet, Construction)
{
	sr::SparseSet<unsigned int> set;

	auto size = set.size();
	auto cap = set.capacity();

	ASSERT_EQ(size, 0);
	ASSERT_EQ(cap, 0);

	sr::SparseSet<unsigned int> set_argument(10);
	
	auto size_a = set_argument.size();
	auto cap_a = set_argument.capacity();

	ASSERT_EQ(size_a, 0);
	ASSERT_EQ(cap_a, 10);
}

TEST(SparseSet, Insertion)
{
	sr::SparseSet<unsigned int> set;

	int a = 10;
	int b = 20;
	int c = 30;

	set.insert(a);
	set.insert(b);
	set.insert(c);

	ASSERT_EQ(set.has(a), true);
	ASSERT_EQ(set.has(b), true);
	ASSERT_EQ(set.has(c), true);
	ASSERT_EQ(set.size(), 3);
}

TEST(SparseSet, Removal)
{
	sr::SparseSet<unsigned int> set;

	int a = 10;
	int b = 20;
	int c = 30;

	set.insert(a);
	set.insert(b);
	set.insert(c);

	set.remove(b);

	ASSERT_EQ(set.has(a), true);
	ASSERT_EQ(set.has(b), false);
	ASSERT_EQ(set.has(c), true);
	ASSERT_EQ(set.size(), 2);
}