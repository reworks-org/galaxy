///
/// ManagerTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>
#include <solar/entity/Manager.hpp>

struct Component
{
	Component(const int value)
	    : val {value}
	{
	}

	int val;
};

struct BlankSystem : public DemoSystem
{
};

struct DemoSystem : public sr::System
{
	DemoSystem(const int val) : sr::System{}
	{
		this->val = val;
	}

	~DemoSystem() override {};
	void events() override
	{
		val = 10;
	}

	void update(pr::ProtectedDouble* dt) override
	{
		val = 20;
	}

	int val = 0;
};

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

	auto names = m.get_debug_name_map();
	EXPECT_TRUE(names.contains("Test"));
}

TEST(Manager, AssignNameExistingEntity)
{
	sr::Manager m;
	auto e = m.create();

	bool res   = m.assign_name(e, "Test");
	auto names = m.get_debug_name_map();
	ASSERT_EQ(res, true);
	EXPECT_TRUE(names.contains("Test"));
}

TEST(Manager, AssignNameNonExistantEntity)
{
	sr::Manager m;
	bool res = m.assign_name(1, "Test");

	auto names = m.get_debug_name_map();
	ASSERT_EQ(res, false);
	EXPECT_FALSE(names.contains("Test"));
}

TEST(Manager, AssignNameDuplicate)
{
	sr::Manager m;
	auto e = m.create();

	EXPECT_TRUE(m.assign_name(e, "Test"));
	EXPECT_FALSE(m.assign_name(e, "Test"));
}

TEST(Manager, FindFromName)
{
	sr::Manager m;
	auto e = m.create("Test");

	auto found = m.find_from_name("Test");
	EXPECT_EQ(found, e);
}

TEST(Manager, FindFromNameNonExistant)
{
	sr::Manager m;

	auto found = m.find_from_name("Test");
	EXPECT_EQ(found, 0);
}

TEST(Manager, GetAllNames)
{
	sr::Manager m;
	auto a = m.create("A");
	auto b = m.create("B");
	auto c = m.create("C");

	auto map = m.get_debug_name_map();
	EXPECT_EQ(map.size(), 3);

	auto foundA = map.find("A");
	auto foundB = map.find("B");
	auto foundC = map.find("C");

	ASSERT_TRUE(foundA != map.end());
	ASSERT_TRUE(foundB != map.end());
	ASSERT_TRUE(foundC != map.end());

	EXPECT_EQ(a, foundA->second);
	EXPECT_EQ(b, foundB->second);
	EXPECT_EQ(c, foundC->second);
}

TEST(Manager, ComponentAddGet)
{
	sr::Manager m;
	auto e = m.create();

	auto* comp = m.create_component<Component>(e, 2);

	ASSERT_TRUE(comp != nullptr);
	EXPECT_EQ(comp->val, 2);

	comp->val   = 10;
	auto* comp2 = m.get<Component>(e);

	ASSERT_TRUE(comp2 != nullptr);
	EXPECT_EQ(comp2->val, 10);
}

TEST(Manager, ComponentRemove)
{
	sr::Manager m;
	auto e     = m.create();
	auto* comp = m.create_component<Component>(e, 1);

	m.remove<Component>(e);
}

TEST(Manager, ComponentRemoveEmpty)
{
	sr::Manager m;
	auto e = m.create();

	// empty because we did not call create_component().
	m.remove<Component>(e);
}

TEST(Manager, ComponentRemoveInvalid)
{
	sr::Manager m;
	m.remove<Component>(0);
}

TEST(Manager, ComponentAddRemoveAdd)
{
	// Remove component then readd it.

	sr::Manager m;
	auto e = m.create();

	auto* comp = m.create_component<Component>(e, 10);
	ASSERT_TRUE(comp != nullptr);
	EXPECT_EQ(comp->val, 10);

	m.remove<Component>(e);
	comp = m.create_component<Component>(e, 20);

	ASSERT_TRUE(comp != nullptr);
	EXPECT_EQ(comp->val, 20);
}

TEST(Manager, ComponentRemoveGet)
{
	sr::Manager m;
	auto e = m.create();

	m.create_component<Component>(e, 1);
	m.remove<Component>(e);

	auto* comp = m.get<Component>(e);
	EXPECT_EQ(comp, nullptr);
}

TEST(Manager, RemoveNonExistingComponent)
{
	sr::Manager m;
	auto e = m.create();

	m.remove<Component>(e);
}

TEST(Manager, RemoveNonExistingComponentWithInvalidEntity)
{
	sr::Manager m;
	m.remove<Component>(0);
}

TEST(Manager, GetNonExistingComponent)
{
	sr::Manager m;
	auto e = m.create();

	auto* comp = m.get<Component>(e);
	EXPECT_EQ(comp, nullptr);
}

TEST(Manager, GetNonExistingComponentWithInvalidEntity)
{
	sr::Manager m;

	auto* comp = m.get<Component>(0);
	EXPECT_EQ(comp, nullptr);
}

TEST(Manager, CreateFromInvalid)
{
	sr::Manager m;
	auto* comp = m.create_component<Component>(0, 1);

	EXPECT_EQ(comp, nullptr);
}

TEST(Manager, Operate)
{

}

TEST(Manager, Destroy)
{
	sr::Manager m;
	auto e1 = m.create();
	auto e2 = m.create();

	m.create_component<Component>(e1, 10);
	m.create_component<Component>(e2, 20);

	m.destroy(e1);
}

TEST(Manager, DestroyInvalid)
{
	sr::Manager m;

	m.destroy(0);
}

TEST(Manager, DestroyEmpty)
{
	sr::Manager m;
	auto e = m.create();

	m.destroy(e);
}

TEST(Manager, CreateGetSystem)
{
	sr::Manager m;
	m.create_system<DemoSystem>(5);
	auto* sys = m.get<DemoSystem>();

	ASSERT_TRUE(sys != nullptr);
	EXPECT_EQ(sys->val, 5);
}

TEST(Manager, GetInvalidSystem)
{
	sr::Manager m;
	auto* sys = m.get<BlankSystem>();

	EXPECT_EQ(sys, nullptr);
}

TEST(Manager, Events)
{
	sr::Manager m;
	m.create_system<DemoSystem>(5);

	m.events();

	auto* sys = m.get<DemoSystem>();

	ASSERT_TRUE(sys != nullptr);
	EXPECT_EQ(sys->val, 10);
}

TEST(Manager, Updates)
{
	sr::Manager m;
	m.create_system<DemoSystem>(5);

	m.update(nullptr);

	auto* sys = m.get<DemoSystem>();

	ASSERT_TRUE(sys != nullptr);
	EXPECT_EQ(sys->val, 20);
}

TEST(Manager, EventsWithNoSystems)
{
	sr::Manager m;
	m.events();
}

TEST(Manager, UpdatesWithNoSystems)
{
	sr::Manager m;
	m.update(nullptr);
}

TEST(Manager, Clear)
{
	sr::Manager m;
	auto e1 = m.create();
	auto e2 = m.create();

	m.create_component<Component>(e1, 10);
	m.create_component<Component>(e2, 20);

	m.clear();
}