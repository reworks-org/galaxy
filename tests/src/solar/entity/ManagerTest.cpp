///
/// ManagerTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>
#include <solar/entity/Manager.hpp>

TEST(Manager, HasEntity)
{
	sr::Manager m;
	auto e = m.create();

	EXPECT_TRUE(m.has(e));
}

TEST(Manager, DoesNotHaveEntity)
{
	sr::Manager m;
	auto e = m.create();

	EXPECT_FALSE(m.has(0));
}

TEST(Manager, HasWithEmptyManager)
{
	sr::Manager m;

	EXPECT_FALSE(m.has(0));
}

TEST(Manager, ValidateValidEntity)
{
	sr::Manager m;
	auto e = m.create();

	EXPECT_TRUE(m.validate(e));
}

TEST(Manager, ValidateInvalidEntity)
{
	sr::Manager m;
	auto e = m.create();

	EXPECT_FALSE(m.validate(0));
	EXPECT_FALSE(m.validate(1));
}


TEST(Manager, ValidateInvalidEntityEmptyManager)
{
	sr::Manager m;

	EXPECT_FALSE(m.validate(0));
	EXPECT_FALSE(m.validate(1));
}

TEST(Manager, Create)
{
	sr::Manager m;
	auto e = m.create();

	EXPECT_TRUE(m.validate(e));
}

TEST(Manager, CreateWithName)
{
	sr::Manager m;
	auto e = m.create("Test");

	EXPECT_TRUE(m.validate(e));
	
	auto names = m.getAllNames();
	auto found = names.find("Test");
	ASSERT_TRUE(found != names.end());
	EXPECT_EQ(found->first, "Test");
}

TEST(Manager, AssignNameExistingEntity)
{
	sr::Manager m;
	auto e = m.create();

	bool res = m.assignName(e, "Test");
	auto names = m.getAllNames();
	auto found = names.find("Test");
	ASSERT_EQ(res, true);
	ASSERT_TRUE(found != names.end());
	EXPECT_EQ(found->first, "Test");
}

TEST(Manager, AssignNameNonExistantEntity)
{
	sr::Manager m;
	bool res = m.assignName(1, "Test");

	auto names = m.getAllNames();
	auto found = names.find("Test");
	EXPECT_EQ(res, false);
	EXPECT_TRUE(found == names.end());
}

TEST(Manager, AssignNameDuplicate)
{
	sr::Manager m;
	auto e = m.create();

	EXPECT_TRUE(m.assignName(e, "Test"));
	EXPECT_FALSE(m.assignName(e, "Test"));
}

TEST(Manager, FindFromName)
{
	sr::Manager m;
	auto e = m.create("Test");

	auto found = m.findFromName("Test");
	EXPECT_EQ(found, e);
}

TEST(Manager, FindFromNameNonExistant)
{
	sr::Manager m;
	
	auto found = m.findFromName("Test");
	EXPECT_EQ(found, 0);
}

TEST(Manager, GetAllNames)
{
	sr::Manager m;
	auto a = m.create("A");
	auto b = m.create("B");
	auto c = m.create("C");

	auto map = m.getAllNames();
	EXPECT_EQ(map.size(), 3);

	auto foundA = map.find("A");
	auto foundB = map.find("B");
	auto foundC = map.find("C");

	EXPECT_EQ(a, foundA->second);
	EXPECT_EQ(b, foundB->second);
	EXPECT_EQ(c, foundC->second);
}

TEST(Manager, ComponentAdd)
{
}

TEST(Manager, ComponentRemove)
{
}

TEST(Manager, ComponentAddRemoveAdd)
{
}

TEST(Manager, ComponentAddGet)
{
}

TEST(Manager, ComponentRemoveGet)
{
}

TEST(Manager, RemoveComponent)
{
}

TEST(Manager, RemoveComponentInvalidEntity)
{
}

TEST(Manager, RemoveNonExistingComponent)
{
}

TEST(Manager, RemoveInvalidComponent)
{
}

TEST(Manager, CreateFromInvalid)
{
	sr::Manager m;
	m.create();
}

TEST(Manager, Operate)
{
}

TEST(Manager, AddSystem)
{

}

TEST(Manager, GetInvalidSystem)
{

}

TEST(Manager, Destroy)
{
}

TEST(Manager, EventsWithNoSystems)
{
}

TEST(Manager, UpdatesWithNoSystems)
{
}

TEST(Manager, Clear)
{
}