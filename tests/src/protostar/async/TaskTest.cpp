///
/// TaskTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>
#include <protostar/async/Task.hpp>

TEST(Task, SetAndExec)
{
	pr::Task task;
	int counter = 0;

	task.set([&]() {
		counter++;
	});

	task.exec();

	ASSERT_TRUE(task.done());
	EXPECT_EQ(counter, 1);
}

TEST(Task, SetAndExecWait)
{
	pr::Task task;
	int counter = 0;

	task.set([&]() {
		counter++;
	});

	task.exec();
	task.waitUntilDone();

	ASSERT_TRUE(task.done());
	EXPECT_EQ(counter, 1);
}