///
/// DispatcherTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>
#include <starlight/Dispatcher.hpp>

struct TestEvent
{
	TestEvent(const int val)
	{
		this->val = val;
	}

	int val = 0;
};

struct Demo
{
	void on_event(const TestEvent& e)
	{
		this->val = e.val;
	}

	int val = 0;
};

TEST(Dispatcher, FullTest)
{
	sl::Dispatcher dispatch;
	Demo demo;

	dispatch.subscribe<TestEvent>(demo);
	dispatch.trigger<TestEvent>(100);

	EXPECT_EQ(demo.val, 100);
}