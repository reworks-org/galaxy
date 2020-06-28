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

TEST(Manager, RemoveEntity)
{
}

TEST(Manager, CreateFromInvalid)
{
	sr::Manager m;
	m.create();
}