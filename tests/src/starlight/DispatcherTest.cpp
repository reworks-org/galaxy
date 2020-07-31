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
	const int val = 100;
};

struct Demo
{
	void on_event(const TestEvent& e)
	{
		this->val = e.val;
	}

	int val = 0;
};

TEST(Dispatcher, x)
{
	sl::Dispatcher dispatch;
	Demo demo;

	dispatch.subscribe<TestEvent>(demo);
}