///
/// ManagerTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>

#include <galaxy/core/World.hpp>

struct Component
{
	Component(const int value)
	    : val {value}
	{
	}

	int val;
};

struct AA
{
	AA()    = default;
	int val = 0;
	AA(AA&& aa)
	{
		this->val = aa.val;
	}

	AA& operator=(AA&& aa)
	{
		if (this != &aa)
		{
			this->val = aa.val;
		}

		return *this;
	}
};

struct BB
{
	BB()    = default;
	int val = 0;
	BB(BB&& bb)
	{
		this->val = bb.val;
	}

	BB& operator=(BB&& bb)
	{
		if (this != &bb)
		{
			this->val = bb.val;
		}

		return *this;
	}
};

struct BlankSystem : public galaxy::ecs::System
{
	void update(galaxy::core::World& world, const double dt) override
	{
	}
};

struct DemoSystem : public galaxy::ecs::System
{
	DemoSystem(const int val)
	    : galaxy::ecs::System {}
	{
		this->val = val;
	}

	~DemoSystem() override {};

	void update(galaxy::core::World& world, const double dt) override
	{
		val = 10;
	}

	int val = 0;
};

TEST(ECS, CreateHasEntity)
{
	galaxy::core::World m;
	auto e = m.create();

	EXPECT_TRUE(m.has(e));
}

TEST(ECS, DoesNotHaveEntity)
{
	galaxy::core::World m;
	auto e = m.create();

	EXPECT_FALSE(m.has(59));
}

TEST(ECS, HasWithEmptyManager)
{
	galaxy::core::World m;

	EXPECT_FALSE(m.has(0));
}

TEST(ECS, ComponentAddGet)
{
	galaxy::core::World m;
	auto e = m.create();

	auto* comp = m.create_component<Component>(e, 2);

	ASSERT_TRUE(comp != nullptr);
	EXPECT_EQ(comp->val, 2);

	comp->val   = 10;
	auto* comp2 = m.get<Component>(e);

	ASSERT_TRUE(comp2 != nullptr);
	EXPECT_EQ(comp2->val, 10);
}

TEST(ECS, ComponentRemove)
{
	galaxy::core::World m;
	auto e     = m.create();
	auto* comp = m.create_component<Component>(e, 1);

	m.remove<Component>(e);
}

TEST(ECS, ComponentRemoveEmpty)
{
	galaxy::core::World m;
	auto e = m.create();

	// empty because we did not call create_component().
	m.remove<Component>(e);
}

TEST(ECS, ComponentRemoveInvalid)
{
	galaxy::core::World m;
	m.remove<Component>(0);
}

TEST(ECS, ComponentAddRemoveAdd)
{
	// Remove component then readd it.

	galaxy::core::World m;
	auto e = m.create();

	auto* comp = m.create_component<Component>(e, 10);
	ASSERT_TRUE(comp != nullptr);
	EXPECT_EQ(comp->val, 10);

	m.remove<Component>(e);
	comp = m.create_component<Component>(e, 20);

	ASSERT_TRUE(comp != nullptr);
	EXPECT_EQ(comp->val, 20);
}

TEST(ECS, ComponentRemoveGet)
{
	galaxy::core::World m;
	auto e = m.create();

	m.create_component<Component>(e, 1);
	m.remove<Component>(e);

	auto* comp = m.get<Component>(e);
	EXPECT_EQ(comp, nullptr);
}

TEST(ECS, RemoveNonExistingComponent)
{
	galaxy::core::World m;
	auto e = m.create();

	m.remove<Component>(e);
}

TEST(ECS, RemoveNonExistingComponentWithInvalidEntity)
{
	galaxy::core::World m;
	m.remove<Component>(0);
}

TEST(ECS, GetNonExistingComponent)
{
	galaxy::core::World m;
	auto e = m.create();

	auto* comp = m.get<Component>(e);
	EXPECT_EQ(comp, nullptr);
}

TEST(ECS, GetNonExistingComponentWithInvalidEntity)
{
	galaxy::core::World m;

	auto* comp = m.get<Component>(0);
	EXPECT_EQ(comp, nullptr);
}

TEST(ECS, CreateFromInvalid)
{
	galaxy::core::World m;
	auto entity = m.create();
	m.destroy(entity);

	auto* comp = m.create_component<Component>(entity, 1);

	EXPECT_EQ(comp, nullptr);
}

TEST(ECS, Operate)
{
	galaxy::core::World m;
	auto e = m.create();
	m.create_component<AA>(e);
	m.create_component<BB>(e);

	m.operate<AA, BB>([](const galaxy::ecs::Entity entity, AA* a, BB* b) {
		a->val = 1;
		b->val = 2;
	});

	auto* a = m.get<AA>(e);
	auto* b = m.get<BB>(e);

	ASSERT_TRUE(a != nullptr);
	ASSERT_TRUE(b != nullptr);

	EXPECT_EQ(a->val, 1);
	EXPECT_EQ(b->val, 2);
}

TEST(ECS, OperateMissing)
{
	galaxy::core::World m;
	auto e = m.create();
	m.create_component<AA>(e);

	auto* a = m.get<AA>(e);
	a->val  = 5;

	ASSERT_TRUE(a != nullptr);
	m.operate<AA, BB>([](const galaxy::ecs::Entity entity, AA* a, BB* b) {
		if (a != nullptr)
		{
			a->val = 1;
		}

		if (b != nullptr)
		{
			b->val = 1;
		}
	});

	// I.e. operate() should not have been called.
	EXPECT_EQ(a->val, 5);
}

TEST(ECS, OperateAddRemove)
{
	galaxy::core::World m;
	auto e = m.create();

	m.create_component<AA>(e);
	m.create_component<BB>(e);

	m.operate<AA, BB>([](const galaxy::ecs::Entity entity, AA* a, BB* b) {
		a->val = 1;
		b->val = 2;
	});

	auto* a = m.get<AA>(e);
	auto* b = m.get<BB>(e);

	ASSERT_TRUE(a != nullptr);
	ASSERT_TRUE(b != nullptr);

	EXPECT_EQ(a->val, 1);
	EXPECT_EQ(b->val, 2);

	m.remove<AA>(e);

	// Should NOT be called.
	m.operate<AA, BB>([](const galaxy::ecs::Entity entity, AA* a, BB* b) {
		if (a != nullptr)
		{
			a->val = 5;
		}

		if (b != nullptr)
		{
			b->val = 5;
		}
	});

	auto* a2 = m.get<AA>(e);
	auto* b2 = m.get<BB>(e);

	ASSERT_TRUE(a2 == nullptr);
	ASSERT_TRUE(b2 != nullptr);

	EXPECT_EQ(b2->val, 2);
}

TEST(ECS, Destroy)
{
	galaxy::core::World m;
	auto e1 = m.create();
	auto e2 = m.create();

	m.create_component<Component>(e1, 10);
	m.create_component<Component>(e2, 20);

	m.destroy(e1);
}

TEST(ECS, DestroyInvalid)
{
	galaxy::core::World m;

	m.destroy(0);
}

TEST(ECS, DestroyEmpty)
{
	galaxy::core::World m;
	auto e = m.create();

	m.destroy(e);
}

TEST(ECS, CreateGetSystem)
{
	galaxy::core::World m;
	m.create_system<DemoSystem>(5);
	auto* sys = m.get_system<DemoSystem>();

	ASSERT_TRUE(sys != nullptr);
	EXPECT_EQ(sys->val, 5);
}

TEST(ECS, GetInvalidSystem)
{
	galaxy::core::World m;

	try
	{
		auto* sys = m.get_system<BlankSystem>();
	}
	catch (...)
	{
		// Should catch an exception.
		EXPECT_TRUE(true);
	}
}

TEST(ECS, Updates)
{
	galaxy::core::World m;
	m.create_system<DemoSystem>(5);

	m.update(0.0);

	auto* sys = m.get_system<DemoSystem>();

	ASSERT_TRUE(sys != nullptr);
	EXPECT_EQ(sys->val, 10);
}

TEST(ECS, UpdatesWithNoSystems)
{
	galaxy::core::World m;
	m.update(0.0);
}

TEST(ECS, Clear)
{
	galaxy::core::World m;
	auto e1 = m.create();
	auto e2 = m.create();

	m.create_component<Component>(e1, 10);
	m.create_component<Component>(e2, 20);

	m.clear();
}