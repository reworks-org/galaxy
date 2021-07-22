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
	void update(galaxy::core::Scene2D* scene, const double dt) override
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

	void update(galaxy::core::Scene2D* scene, const double dt) override
	{
		val = 10;
	}

	int val = 0;
};

TEST(ECS, CreateHasEntity)
{
	galaxy::core::World w;
	auto e = w.create();

	EXPECT_TRUE(w.has(e));
}

TEST(ECS, DoesNotHaveEntity)
{
	galaxy::core::World w;
	auto e = w.create();

	EXPECT_FALSE(w.has(59));
}

TEST(ECS, HasWithEmptyManager)
{
	galaxy::core::World w;

	EXPECT_FALSE(w.has(0));
}

TEST(ECS, ComponentAddGet)
{
	galaxy::core::World w;
	auto e = w.create();

	auto* comp = w.create_component<Component>(e, 2);

	ASSERT_TRUE(comp != nullptr);
	EXPECT_EQ(comp->val, 2);

	comp->val   = 10;
	auto* comp2 = w.get<Component>(e);

	ASSERT_TRUE(comp2 != nullptr);
	EXPECT_EQ(comp2->val, 10);
}

TEST(ECS, ComponentRemove)
{
	galaxy::core::World w;
	auto e     = w.create();
	auto* comp = w.create_component<Component>(e, 1);

	w.remove<Component>(e);
}

TEST(ECS, ComponentRemoveEmpty)
{
	galaxy::core::World w;
	auto e = w.create();

	// empty because we did not call create_component().
	w.remove<Component>(e);
}

TEST(ECS, ComponentRemoveInvalid)
{
	galaxy::core::World w;
	w.remove<Component>(0);
}

TEST(ECS, ComponentAddRemoveAdd)
{
	// Remove component then readd it.

	galaxy::core::World w;
	auto e = w.create();

	auto* comp = w.create_component<Component>(e, 10);
	ASSERT_TRUE(comp != nullptr);
	EXPECT_EQ(comp->val, 10);

	w.remove<Component>(e);
	comp = w.create_component<Component>(e, 20);

	ASSERT_TRUE(comp != nullptr);
	EXPECT_EQ(comp->val, 20);
}

TEST(ECS, ComponentRemoveGet)
{
	galaxy::core::World w;
	auto e = w.create();

	w.create_component<Component>(e, 1);
	w.remove<Component>(e);

	auto* comp = w.get<Component>(e);
	EXPECT_EQ(comp, nullptr);
}

TEST(ECS, RemoveNonExistingComponent)
{
	galaxy::core::World w;
	auto e = w.create();

	w.remove<Component>(e);
}

TEST(ECS, RemoveNonExistingComponentWithInvalidEntity)
{
	galaxy::core::World w;
	w.remove<Component>(0);
}

TEST(ECS, GetNonExistingComponent)
{
	galaxy::core::World w;
	auto e = w.create();

	auto* comp = w.get<Component>(e);
	EXPECT_EQ(comp, nullptr);
}

TEST(ECS, GetNonExistingComponentWithInvalidEntity)
{
	galaxy::core::World w;

	auto* comp = w.get<Component>(0);
	EXPECT_EQ(comp, nullptr);
}

TEST(ECS, CreateFromInvalid)
{
	galaxy::core::World w;
	auto entity = w.create();
	w.destroy(entity);

	auto* comp = w.create_component<Component>(entity, 1);

	EXPECT_EQ(comp, nullptr);
}

TEST(ECS, Operate)
{
	galaxy::core::World w;
	auto e = w.create();
	w.enable(e);
	w.create_component<AA>(e);
	w.create_component<BB>(e);

	w.operate<AA, BB>(
		[](const galaxy::ecs::Entity entity, AA* a, BB* b)
		{
			a->val = 1;
			b->val = 2;
		});

	auto* a = w.get<AA>(e);
	auto* b = w.get<BB>(e);

	ASSERT_TRUE(a != nullptr);
	ASSERT_TRUE(b != nullptr);

	EXPECT_EQ(a->val, 1);
	EXPECT_EQ(b->val, 2);
}

TEST(ECS, OperateMissing)
{
	galaxy::core::World w;
	auto e = w.create();
	w.enable(e);
	w.create_component<AA>(e);

	auto* a = w.get<AA>(e);
	a->val  = 5;

	ASSERT_TRUE(a != nullptr);
	w.operate<AA, BB>(
		[](const galaxy::ecs::Entity entity, AA* a, BB* b)
		{
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
	galaxy::core::World w;
	auto e = w.create();
	w.enable(e);

	w.create_component<AA>(e);
	w.create_component<BB>(e);

	w.operate<AA, BB>(
		[](const galaxy::ecs::Entity entity, AA* a, BB* b)
		{
			a->val = 1;
			b->val = 2;
		});

	auto* a = w.get<AA>(e);
	auto* b = w.get<BB>(e);

	ASSERT_TRUE(a != nullptr);
	ASSERT_TRUE(b != nullptr);

	EXPECT_EQ(a->val, 1);
	EXPECT_EQ(b->val, 2);

	w.remove<AA>(e);

	// Should NOT be called.
	w.operate<AA, BB>(
		[](const galaxy::ecs::Entity entity, AA* a, BB* b)
		{
			if (a != nullptr)
			{
				a->val = 5;
			}

			if (b != nullptr)
			{
				b->val = 5;
			}
		});

	auto* a2 = w.get<AA>(e);
	auto* b2 = w.get<BB>(e);

	ASSERT_TRUE(a2 == nullptr);
	ASSERT_TRUE(b2 != nullptr);

	EXPECT_EQ(b2->val, 2);
}

TEST(ECS, Destroy)
{
	galaxy::core::World w;
	auto e1 = w.create();
	auto e2 = w.create();

	w.create_component<Component>(e1, 10);
	w.create_component<Component>(e2, 20);

	w.destroy(e1);

	EXPECT_EQ(w.has(e1), false);

	auto* component = w.get<Component>(e1);
	EXPECT_EQ(component, nullptr);
}

TEST(ECS, DestroyInvalid)
{
	galaxy::core::World w;

	w.destroy(0);
}

TEST(ECS, DestroyEmpty)
{
	galaxy::core::World w;
	auto e = w.create();

	w.destroy(e);
	EXPECT_EQ(w.has(e), false);
}

TEST(ECS, CreateGetSystem)
{
	galaxy::core::World w;
	w.create_system<DemoSystem>(5);
	auto* sys = w.get_system<DemoSystem>();

	ASSERT_TRUE(sys != nullptr);
	EXPECT_EQ(sys->val, 5);
}

TEST(ECS, GetInvalidSystem)
{
	galaxy::core::World w;

	try
	{
		auto* sys = w.get_system<BlankSystem>();
	}
	catch (...)
	{
		// Should catch an exception.
		EXPECT_TRUE(true);
	}
}

TEST(ECS, Updates)
{
	galaxy::core::World w;
	w.create_system<DemoSystem>(5);

	w.update(nullptr, 0.0);

	auto* sys = w.get_system<DemoSystem>();

	ASSERT_TRUE(sys != nullptr);
	EXPECT_EQ(sys->val, 10);
}

TEST(ECS, Clear)
{
	galaxy::core::World w;
	auto e1 = w.create();
	auto e2 = w.create();

	w.create_component<Component>(e1, 10);
	w.create_component<Component>(e2, 20);

	w.clear();
}

TEST(ECS, Add_Is_Get_Parent)
{
	galaxy::core::World w;
	auto entity = w.create();

	w.make_parent(entity);

	EXPECT_EQ(w.is_parent(entity), true);
	EXPECT_FALSE(w.get_children(entity) == nullptr);
}

TEST(ECS, Remove_Is_Get_Parent)
{
	galaxy::core::World w;
	auto entity = w.create();

	w.make_parent(entity);
	w.remove_parent(entity);

	EXPECT_EQ(w.is_parent(entity), false);
	EXPECT_EQ(w.get_children(entity), nullptr);
}

TEST(ECS, AssignChild)
{
	galaxy::core::World w;
	auto p = w.create();
	auto c = w.create();

	w.make_parent(p);
	w.assign_child(p, c);

	EXPECT_EQ(w.is_parent(p), true);

	ASSERT_FALSE(w.get_children(p) == nullptr);
	ASSERT_FALSE(w.get_children(p)->empty());
	EXPECT_TRUE(w.get_children(p)->at(0) == c);
}

TEST(ECS, RemoveChild)
{
	galaxy::core::World w;
	auto p = w.create();
	auto c = w.create();

	w.make_parent(p);
	w.assign_child(p, c);

	EXPECT_EQ(w.is_parent(p), true);

	ASSERT_FALSE(w.get_children(p) == nullptr);
	ASSERT_TRUE(w.get_children(p)->at(0) == c);

	w.remove_child(p, c);

	EXPECT_TRUE(w.get_children(p)->empty());
}

TEST(ECS, Destroy_Is_Get_Parent)
{
	galaxy::core::World w;
	auto entity = w.create();

	w.make_parent(entity);
	w.create_component<Component>(entity, 0);

	w.destroy_parent(entity);

	EXPECT_EQ(w.has(entity), false);
	EXPECT_EQ(w.is_parent(entity), false);
	EXPECT_EQ(w.get_children(entity), nullptr);

	auto* component = w.get<Component>(entity);
	EXPECT_EQ(component, nullptr);
}

TEST(ECS, DestroyParentNotChildren)
{
	galaxy::core::World w;
	auto e1 = w.create();

	w.make_parent(e1);
	w.create_component<Component>(e1, 0);

	w.destroy(e1);

	EXPECT_EQ(w.has(e1), false);
	EXPECT_EQ(w.is_parent(e1), false);

	auto* component = w.get<Component>(e1);
	EXPECT_EQ(component, nullptr);
}

TEST(ECS, DestroyChildNotParent)
{
	galaxy::core::World w;
	auto p = w.create();
	auto c = w.create();

	w.make_parent(p);
	w.assign_child(p, c);

	EXPECT_EQ(w.is_parent(p), true);

	ASSERT_FALSE(w.get_children(p) == nullptr);
	EXPECT_TRUE(w.get_children(p)->at(0) == c);

	w.destroy(c);

	EXPECT_EQ(w.is_parent(p), true);
	ASSERT_FALSE(w.get_children(p) == nullptr);

	EXPECT_TRUE(w.get_children(p)->empty());
	EXPECT_FALSE(w.has(c));
	EXPECT_TRUE(w.has(p));
}