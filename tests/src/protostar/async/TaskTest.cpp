///
/// TaskTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>
#include <protostar/async/Task.hpp>

TEST(Task, set_and_exec)
{
	pr::Task task;
	int counter = 0;

	task.set([&]() {
		counter++;
	});

	task.exec();

	ASSERT_TRUE(task.is_done());
	EXPECT_EQ(counter, 1);
}

TEST(Task, set_exec_and_wait)
{
	pr::Task task;
	int counter = 0;

	task.set([&]() {
		counter++;
	});

	task.exec();
	task.wait_until_done();

	ASSERT_TRUE(task.is_done());
	EXPECT_EQ(counter, 1);
}