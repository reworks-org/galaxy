///
/// TimeTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <chrono>

#include <BS_thread_pool.hpp>
#include <entt/locator/locator.hpp>
#include <gtest/gtest.h>

#include <galaxy/time/Stopwatch.hpp>
#include <galaxy/time/Timer.hpp>

using namespace std::chrono_literals;

TEST(Stopwatch, Run)
{
	int a = 0;

	galaxy::Stopwatch s;
	s.set(
		[&]() {
			a += 1;
		},
		1000
	);

	s.start();
	std::this_thread::sleep_for(1s);

	s.update();
	ASSERT_EQ(a, 1);
}

TEST(Stopwatch, Stop)
{
	int a = 0;

	galaxy::Stopwatch s;
	s.set(
		[&]() {
			a += 1;
		},
		1000
	);

	s.start();
	s.stop();

	std::this_thread::sleep_for(1s);

	s.update();
	ASSERT_EQ(a, 0);
}

TEST(Stopwatch, Pause)
{
	int a = 0;

	galaxy::Stopwatch s;
	s.set(
		[&]() {
			a += 1;
		},
		1000
	);

	s.start();
	s.pause();

	std::this_thread::sleep_for(1s);

	s.update();
	ASSERT_EQ(a, 0);
}

TEST(Stopwatch, Unpause)
{
	int a = 0;

	galaxy::Stopwatch s;
	s.set(
		[&]() {
			a += 1;
		},
		1000
	);

	s.start();
	s.pause();
	std::this_thread::sleep_for(1s);

	s.unpause();
	std::this_thread::sleep_for(1s);

	s.update();
	ASSERT_EQ(a, 1);
}

TEST(Timer, Run)
{
	entt::locator<BS::light_thread_pool>::emplace(2);

	{
		int a = 0;

		galaxy::Timer s;
		s.set(
			[&]() {
				a += 1;
			},
			1000
		);

		s.start();
		std::this_thread::sleep_for(2s);

		ASSERT_EQ(a, 1);
	}

	entt::locator<BS::light_thread_pool>::reset();
}

TEST(Timer, Stop)
{
	entt::locator<BS::light_thread_pool>::emplace(2);

	{
		int a = 0;

		galaxy::Timer s;
		s.set(
			[&]() {
				a += 1;
			},
			200
		);

		s.start(true);

		std::this_thread::sleep_for(1s);
		s.stop();

		ASSERT_TRUE(a >= 4);
	}

	entt::locator<BS::light_thread_pool>::reset();
}

TEST(Timer, Pause)
{
	entt::locator<BS::light_thread_pool>::emplace(2);

	{
		int a = 0;

		galaxy::Timer s;
		s.set(
			[&]() {
				a += 1;
			},
			200
		);

		s.start(true);
		s.pause(true);

		ASSERT_TRUE(a <= 2);
	}

	entt::locator<BS::light_thread_pool>::reset();
}

TEST(Timer, Unpause)
{
	entt::locator<BS::light_thread_pool>::emplace(2);

	{
		int a = 0;

		galaxy::Timer s;
		s.set(
			[&]() {
				a += 1;
			},
			200
		);

		s.start(true);
		s.pause(true);

		ASSERT_TRUE(a <= 2);

		s.pause(false);
		std::this_thread::sleep_for(1s);
		s.stop();

		ASSERT_TRUE(a >= 4);
	}

	entt::locator<BS::light_thread_pool>::reset();
}
