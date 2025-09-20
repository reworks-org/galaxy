///
/// SceneTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>

#include <galaxy/scene/World.hpp>

TEST(World, SceneAdd)
{
	galaxy::World world;

	auto added = world.add("test");
	ASSERT_TRUE(added);
	ASSERT_TRUE(added->name() == "test");
}

TEST(World, SceneGet)
{
	galaxy::World world;

	world.add("test");
	auto get = world.get("test");
	ASSERT_TRUE(get->name() == "test");
}

TEST(World, SceneHas)
{
	galaxy::World world;

	world.add("test");
	ASSERT_TRUE(world.has("test"));
}

TEST(World, SceneRemove)
{
	galaxy::World world;

	world.add("test");
	world.remove("test");

	ASSERT_FALSE(world.has("test"));
}

TEST(World, Push)
{
	galaxy::World world;

	world.add("test");
	world.push("test");

	ASSERT_TRUE(world.top()->name() == "test");
}

TEST(World, Pop)
{
	galaxy::World world;

	world.add("test");
	world.push("test");

	world.pop();
	ASSERT_EQ(world.stack().size(), 0);
	ASSERT_TRUE(world.top() == nullptr);
}

TEST(World, PopAll)
{
	galaxy::World world;

	world.add("test");
	world.add("test2");

	world.push("test");
	world.push("test2");

	world.pop_all();
	ASSERT_EQ(world.stack().size(), 0);
	ASSERT_TRUE(world.top() == nullptr);
}

TEST(World, Clear)
{
	galaxy::World world;

	world.add("test");
	world.add("test2");

	world.push("test");
	world.push("test2");

	world.clear();
	ASSERT_EQ(world.storage().size(), 0);
	ASSERT_EQ(world.stack().size(), 0);
}
