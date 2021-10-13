///
/// ThreadPoolTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>

#include <galaxy/async/ThreadPool.hpp>

TEST(Async, ThreadPool)
{
	std::atomic<int>          count = 0;
	galaxy::async::ThreadPool pool;
	galaxy::async::Task       task;
	task.set([&]() {
		count = 2;
	});

	pool.queue(&task);
	task.wait_until_done();
	pool.finish();

	EXPECT_EQ(count.load(), 2);
}