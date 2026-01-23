///
/// SceneTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>

#include <galaxy/scene/SceneManager.hpp>

TEST(SceneManager, SceneAdd)
{
	galaxy::SceneManager sm;

	auto added = sm.add("test");
	ASSERT_TRUE(added);
	ASSERT_TRUE(added->name() == "test");
}

TEST(SceneManager, SceneGet)
{
	galaxy::SceneManager sm;

	sm.add("test");
	auto get = sm.get("test");
	ASSERT_TRUE(get->name() == "test");
}

TEST(SceneManager, SceneHas)
{
	galaxy::SceneManager sm;

	sm.add("test");
	ASSERT_TRUE(sm.has("test"));
}

TEST(SceneManager, SceneRemove)
{
	galaxy::SceneManager sm;

	sm.add("test");
	sm.remove("test");

	ASSERT_FALSE(sm.has("test"));
}

TEST(SceneManager, Push)
{
	galaxy::SceneManager sm;

	sm.add("test");
	sm.push("test");

	ASSERT_TRUE(sm.top()->name() == "test");
}

TEST(SceneManager, Pop)
{
	galaxy::SceneManager sm;

	sm.add("test");
	sm.push("test");

	sm.pop();
	ASSERT_EQ(sm.stack().size(), 0);
	ASSERT_TRUE(sm.top() == nullptr);
}

TEST(SceneManager, PopAll)
{
	galaxy::SceneManager sm;

	sm.add("test");
	sm.add("test2");

	sm.push("test");
	sm.push("test2");

	sm.pop_all();
	ASSERT_EQ(sm.stack().size(), 0);
	ASSERT_TRUE(sm.top() == nullptr);
}

TEST(SceneManager, Clear)
{
	galaxy::SceneManager sm;

	sm.add("test");
	sm.add("test2");

	sm.push("test");
	sm.push("test2");

	sm.clear();
	ASSERT_EQ(sm.storage().size(), 0);
	ASSERT_EQ(sm.stack().size(), 0);
}
