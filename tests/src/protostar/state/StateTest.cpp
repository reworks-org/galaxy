///
/// StateTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>
#include <protostar/state/State.hpp>

class Demo : public pr::State
{
public:
	inline Demo() noexcept
	    : State("Demo")
	{
	}

	inline ~Demo() noexcept override
	{
	}

	inline void on_push() override
	{
	}

	inline void on_pop() override
	{
	}

	inline void events() override
	{
	}

	inline void update(pr::ProtectedDouble* dt) override
	{
	}

	inline void render() override
	{
	}
};

TEST(State, get_name)
{
	Demo demo;
	EXPECT_EQ(demo.get_name(), "Demo");
}

TEST(State, set_name)
{
	Demo demo;
	demo.set_name("testing 1 2 3");

	EXPECT_EQ(demo.get_name(), "testing 1 2 3");
}