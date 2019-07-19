///
/// ManagerTest.cpp
///
/// solar
/// See LICENSE.txt.
///

#include <solar/entity/Manager.hpp>

#include "gtest/gtest.h"

struct TestA
{
	unsigned int val = 100;
};

struct TestB
{
	TestB(unsigned int v)
	{
		val = v;
	}

	unsigned int val = 200;
};

TEST(Manager, EntityCreateDestroy)
{
	sr::Manager manager;

	sr::Entity e0 = manager.create();
	sr::Entity e1 = manager.create();
	sr::Entity e2 = manager.create();

	// Make sure count is incremental.
	ASSERT_NE(e0, e1);
	
	ASSERT_GT(e1, e0);

	ASSERT_TRUE(manager.validate(e0));
	ASSERT_TRUE(manager.validate(e1));
	ASSERT_TRUE(manager.validate(e2));

	// fix 1 being a valid entity...

	SR_INTEGER notRealEntityA = 0;
	//SR_INTEGER notRealEntityB = 1;
	SR_INTEGER notRealEntityC = 2;
	ASSERT_FALSE(manager.validate(notRealEntityA));
	//ASSERT_FALSE(manager.validate(notRealEntityB));
	ASSERT_FALSE(manager.validate(notRealEntityC));

	ASSERT_EQ(manager.has(e0), true);
	ASSERT_EQ(manager.has(e1), true);
	ASSERT_EQ(manager.has(e2), true);

	manager.destroy(e1);

	ASSERT_EQ(manager.has(e1), false);
}

TEST(Manager, ComponentAddGetDestroy)
{
	sr::Manager manager;

	sr::Entity e0 = manager.create();
	sr::Entity e1 = manager.create();
	sr::Entity e2 = manager.create();

	manager.add<TestA>(e0);
	manager.add<TestB>(e1, 300);
	manager.add<TestA>(e2);
	manager.add<TestB>(e2, 400);

	// scope to test data manipulation
	{
		auto testa = manager.get<TestA>(e0);
		auto testb = manager.get<TestB>(e1);

		ASSERT_EQ(testa->val, 100);
		ASSERT_EQ(testb->val, 300);

		testa->val = 800;
		testb->val = 900;
	}
	
	auto testa2 = manager.get<TestA>(e0);
	auto testb2 = manager.get<TestB>(e1);

	ASSERT_EQ(testa2->val, 800);
	ASSERT_EQ(testb2->val, 900);

	{
		auto[e2a, e2b] = manager.multi<TestA, TestB>(e2);

		ASSERT_EQ(e2a->val, 100);
		ASSERT_EQ(e2b->val, 400);

		e2a->val = 600;
		e2b->val = 700;
	}
	
	auto[e2a2, e2b2] = manager.multi<TestA, TestB>(e2);

	ASSERT_EQ(e2a2->val, 600);
	ASSERT_EQ(e2b2->val, 700);
}

TEST(Manager, ComponentOperate)
{
	sr::Manager manager;

	sr::Entity e0 = manager.create();
	sr::Entity e1 = manager.create();
	sr::Entity e2 = manager.create();

	manager.add<TestA>(e0);
	manager.add<TestB>(e1, 100);
	manager.add<TestA>(e2);
	manager.add<TestB>(e2, 200);

	manager.operate<TestA>([](sr::Entity e, TestA* a)
	{
		a->val = 50;
	});

	auto testae0 = manager.get<TestA>(e0);

	ASSERT_EQ(testae0->val, 50);
	
	manager.operate<TestA, TestB>([](sr::Entity e, TestA* a, TestB* b)
	{
		a->val = 25;
		b->val = 75;
	});

	auto testbe1 = manager.get<TestB>(e1);
	auto testbe2 = manager.get<TestB>(e2);

	ASSERT_EQ(testae0->val, 50);
	ASSERT_EQ(testbe1->val, 100);
	ASSERT_EQ(testbe2->val, 75);
}