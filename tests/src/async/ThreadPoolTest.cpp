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
	galaxy::async::ThreadPool pool;

	auto taskA = std::make_shared<galaxy::async::Task>();
	auto taskB = std::make_shared<galaxy::async::Task>();

	std::atomic<int> count     = 0;
	std::atomic<bool> is_false = true;

	taskA->set([&]() {
		count = 1;
	});

	taskB->set([&]() {
		is_false = false;
	});

	pool.queue(taskA);
	pool.queue(taskB);

	taskA->wait_until_done();
	taskB->wait_until_done();

	pool.stop();

	EXPECT_EQ(count.load(), 1);
	EXPECT_EQ(is_false.load(), false);
}