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
	abc(const std::string& name)
		: System(name)
	{
	}

	abc() = delete;

	void update(galaxy::Registry& registry)
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
