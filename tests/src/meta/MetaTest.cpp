///
/// MetaTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>

#include <galaxy/meta/SystemFactory.hpp>

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

TEST(SystemFactory, RegisterAndCreate)
{
	galaxy::SystemFactory factory;
	factory.register_system<abc>("abc");

	galaxy::SystemStack stack;
	factory.create_system("abc", stack);

	ASSERT_FALSE(stack.empty());
	ASSERT_FALSE(stack[0] == nullptr);

	ASSERT_TRUE(stack[0]->id() == "abc");
}
