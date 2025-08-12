///
/// main.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <chrono>
#include <thread>

#include <gtest/gtest.h>
#include <mimalloc.h>
#include <mimalloc-new-delete.h>

using namespace std::chrono_literals;

int main(int argsc, char* argsv[])
{
	mi_version();
	
	testing::InitGoogleTest(&argsc, argsv);
	RUN_ALL_TESTS();

	// Leaves window open long enough to look at the results, without blocking automated builds.
	std::this_thread::sleep_for(8s);

	return 0;
}