///
/// TimerTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <chrono>
#include <thread>

#include <BS_thread_pool.hpp>
#include <gtest/gtest.h>

#include <galaxy/async/Timer.hpp>
#include <galaxy/async/TimerAsync.hpp>
#include <galaxy/core/ServiceLocator.hpp>

using namespace std::chrono_literals;

TEST(Async, Timer)
{
	std::atomic<bool> is_false = true;

	galaxy::async::Timer timer(
		[&]() {
			is_false = false;
		},
		100);

	timer.repeat(false);
	timer.start();

	while (!timer.stopped())
	{
		timer.update();
	}

	EXPECT_EQ(is_false.load(), false);
}

TEST(Async, TimerAsync)
{
	std::atomic<bool> is_false = true;
	galaxy::core::ServiceLocator<BS::thread_pool>::make(2);

	galaxy::async::TimerAsync timer(
		[&]() {
			is_false = false;
		},
		100);

	timer.repeat(false);
	timer.start();

	// Give thread plenty of time to finish.
	std::this_thread::sleep_for(0.5s);

	galaxy::core::ServiceLocator<BS::thread_pool>::del();

	EXPECT_EQ(is_false.load(), false);
}