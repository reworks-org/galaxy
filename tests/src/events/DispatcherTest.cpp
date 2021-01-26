///
/// DispatcherTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>

#include <galaxy/events/dispatcher/Dispatcher.hpp>

struct DemoEvent
{
	DemoEvent(const int val)
	{
		m_val = val;
	}

	int m_val = 0;
};

struct DemoObject
{
	DemoObject()
	{
		m_val = 0;
	}

	void on_event(const DemoEvent& e)
	{
		m_val = e.m_val;
	}

	int m_val;
};

TEST(Dispatcher, SubscribeAndTrigger)
{
	galaxy::events::Dispatcher dispatch;
	DemoObject obj;

	dispatch.subscribe<DemoEvent>(obj);
	dispatch.trigger<DemoEvent>(100);

	EXPECT_EQ(obj.m_val, 100);
}