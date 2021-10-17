///
/// main.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <chrono>
#include <thread>

#include <gtest/gtest.h>

using namespace std::chrono_literals;

int main(int argsc, char* argsv[])
{
	testing::InitGoogleTest(&argsc, argsv);
	const auto result = RUN_ALL_TESTS();

	// Leaves window open long enough to look at the results.
	std::this_thread::sleep_for(8s);
	return result;
}