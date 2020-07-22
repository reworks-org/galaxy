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
	inline void onPush() override
	{
	}
	inline void onPop() override
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

TEST(State, Name)
{
	Demo demo;
	EXPECT_EQ(demo.getName(), "Demo");
}

TEST(State, SetName)
{
	Demo demo;
	demo.set_name("testing 1 2 3");

	EXPECT_EQ(demo.getName(), "testing 1 2 3");
}