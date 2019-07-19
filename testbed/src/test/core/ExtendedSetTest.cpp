///
/// ExtendedSetTest.cpp
///
/// solar
/// See LICENSE.txt.
///

#include <solar/entity/Manager.hpp>

#include "gtest/gtest.h"

///
/// Demo struct A.
///
struct A
{
	A(int _a)
	{
		a = _a;
	}

	unsigned int a = 1;
};

///
/// Demo struct B.
///
struct B
{
	B(int _b)
	{
		b = _b;
	}

	unsigned int b = 2;
};

TEST(ExtendedSet, Add)
{
	// Test used to ensure components are stored and accessed correctly.

	sr::Manager m;
	sr::Entity entity = m.create();

	sr::ExtendedSet<A> es;
	A* a = es.add(entity, 10);

	ASSERT_NE(a, nullptr);
	ASSERT_EQ(a->a, 10);

	A* a2 = es.get(entity);

	ASSERT_NE(a2, nullptr);
	ASSERT_EQ(a2->a, 10);
}

TEST(ExtendedSet, Remove)
{
	// Test to ensure components are deleted correctly.
	sr::Manager m;
	sr::Entity entity = m.create();

	sr::ExtendedSet<B> es;
	B* b = es.add(entity, 20);

	es.remove(entity);

	ASSERT_FALSE(es.has(entity));
}