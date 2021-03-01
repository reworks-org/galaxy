///
/// main.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <chrono>
#include <thread>

#include <gtest/gtest.h>

#include <galaxy/error/Log.hpp>

using namespace std::chrono_literals;

int main(int argsc, char* argsv[])
{
	testing::InitGoogleTest(&argsc, argsv);
	const auto result = RUN_ALL_TESTS();

	// Let us look at the results before closing.
	std::this_thread::sleep_for(5s);
	return result;
}