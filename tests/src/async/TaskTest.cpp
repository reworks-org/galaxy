///
/// TaskTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>

#include <galaxy/async/Task.hpp>

TEST(Async, TaskSetAndExec)
{
	galaxy::async::Task task;
	int counter = 0;

	task.set([&]() {
		counter++;
	});

	task.exec();

	ASSERT_TRUE(task.is_done());
	EXPECT_EQ(counter, 1);
}

TEST(Async, TaskSetExecAndWait)
{
	galaxy::async::Task task;
	int counter = 0;

	task.set([&]() {
		counter++;
	});

	task.exec();
	task.wait_until_done();

	ASSERT_TRUE(task.is_done());
	EXPECT_EQ(counter, 1);
}