///
/// SystemTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>

#include <galaxy/meta/SystemFactory.hpp>
#include <galaxy/scene/SceneManager.hpp>
#include <galaxy/systems/SystemManager.hpp>

struct abc : galaxy::System
{
	abc()                      = delete;
	abc& operator=(const abc&) = delete;
	abc(const abc&)            = delete;
	abc(abc&&)                 = default;
	abc& operator=(abc&&)      = default;
	virtual ~abc()             = default;

	virtual void update(galaxy::EntityManager& em, galaxy::Scene* scene) override
	{
	}

	abc(const std::string& name)
		: System(name)
	{
	}
};

TEST(System, Add)
{
	auto& sf = entt::locator<galaxy::SystemFactory>::emplace();
	sf.register_system<abc>("abc");

	galaxy::SceneManager sm;

	auto added = sm.add("test");
	added->sys_man().add_system("abc");

	ASSERT_TRUE(added->sys_man().stack().size() > 0);
	ASSERT_TRUE(added->sys_man().stack().at(0)->id() == "abc");
}

TEST(System, Clear)
{
	auto& sf = entt::locator<galaxy::SystemFactory>::emplace();
	sf.register_system<abc>("abc");

	galaxy::SceneManager sm;

	auto added = sm.add("test");
	added->sys_man().add_system("abc");

	added->sys_man().clear();
	ASSERT_TRUE(added->sys_man().stack().size() == 0);
}
