///
/// StateMachineTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>
#include <protostar/state/StateMachine.hpp>

class Demo : public pr::State
{
public:
	inline Demo(const int val) noexcept
		:State("Demo"), m_val(val)
	{
	}

	inline ~Demo() noexcept override {}
	inline void onPush() override {}
	inline void onPop() override {}
	inline void events() override
	{
		m_val++;
	}

	inline void update(pr::ProtectedDouble* dt) override
	{
		m_val++;
	}

	inline void render() override
	{
		m_val++;
	}

	inline int get() noexcept
	{
		return m_val;
	}

private:
	int m_val;
};

class Demo2 : public pr::State
{
public:
	inline Demo2(const int val) noexcept
		:State("Demo2"), m_val(val)
	{
	}

	inline ~Demo2() noexcept override {}
	inline void onPush() override {}
	inline void onPop() override {}
	inline void events() override
	{
	}

	inline void update(pr::ProtectedDouble* dt) override
	{
	}

	inline void render() override
	{
	}

	inline int get() noexcept
	{
		return m_val;
	}

private:
	int m_val;
};

TEST(StateMachine, Create)
{
	pr::StateMachine sm;
	auto* state = sm.create<Demo>("Demo", 1);

	EXPECT_EQ(state->get(), 1);
	EXPECT_EQ(state->getName(), "Demo");
}

TEST(StateMachine, PushAndTop)
{
	pr::StateMachine sm;
	sm.create<Demo>("Demo", 1);
	sm.push("Demo");

	auto* top = sm.top<Demo>();

	ASSERT_TRUE(top != nullptr);
	EXPECT_EQ(top->get(), 1);
	EXPECT_EQ(top->getName(), "Demo");
}

TEST(StateMachine, EmptyTop)
{
	pr::StateMachine sm;

	auto* top = sm.top<Demo>();
	EXPECT_EQ(top, nullptr);
}

TEST(StateMachine, Events)
{
	pr::StateMachine sm;
	sm.create<Demo>("Demo", 1);
	sm.push("Demo");

	auto* top = sm.top<Demo>();
	ASSERT_TRUE(top != nullptr);

	sm.events();
	EXPECT_EQ(top->get(), 2);
}

TEST(StateMachine, Updates)
{
	pr::StateMachine sm;
	sm.create<Demo>("Demo", 1);
	sm.push("Demo");

	auto* top = sm.top<Demo>();
	ASSERT_TRUE(top != nullptr);

	sm.events();
	EXPECT_EQ(top->get(), 2);

	sm.update(nullptr);
	EXPECT_EQ(top->get(), 3);
}

TEST(StateMachine, Render)
{
	pr::StateMachine sm;
	sm.create<Demo>("Demo", 1);
	sm.push("Demo");

	auto* top = sm.top<Demo>();
	ASSERT_TRUE(top != nullptr);

	sm.events();
	EXPECT_EQ(top->get(), 2);

	sm.update(nullptr);
	EXPECT_EQ(top->get(), 3);

	sm.render();
	EXPECT_EQ(top->get(), 4);
}

TEST(StateMachine, Pop)
{
	pr::StateMachine sm;
	sm.create<Demo>("Demo", 1);
	sm.create<Demo2>("Demo2", 2);

	sm.push("Demo");
	sm.push("Demo2");

	auto* top2 = sm.top<Demo2>();
	ASSERT_TRUE(top2 != nullptr);

	EXPECT_EQ(top2->get(), 2);

	sm.pop();

	auto* top = sm.top<Demo>();
	ASSERT_TRUE(top != nullptr);

	EXPECT_EQ(top->get(), 1);
}

TEST(StateMachine, Clear)
{
	pr::StateMachine sm;
	sm.create<Demo>("Demo", 1);
	sm.create<Demo2>("Demo2", 2);

	sm.push("Demo");
	sm.push("Demo2");

	sm.clear();
	
	auto* top = sm.top<Demo>();
	EXPECT_EQ(top, nullptr);
}