///
/// ThreadPoolTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>
#include <protostar/async/ThreadPool.hpp>

TEST(ThreadPool, full_test)
{
	std::atomic<int> count = 0;
	pr::ThreadPool pool;
	pr::Task task;
	task.set([&]() {
		count = 2;
	});

	pool.create(1);
	pool.queue(&task);
	pool.start();

	task.wait_until_done();

	pool.end();

	EXPECT_EQ(count.load(), 2);
}