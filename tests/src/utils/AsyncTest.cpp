///
/// StringUtilsTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>

#include <galaxy/utils/Async.hpp>

TEST(Async, IsWorkDone)
{
	std::future<int> future = std::async(std::launch::async, [] {
		std::this_thread::sleep_for(1s);
		return 3;
	});

	do
	{
		std::this_thread::sleep_for(100ms);
	} while (!galaxy::async::is_work_done(future));

	ASSERT_EQ(future.get(), 3);
}
