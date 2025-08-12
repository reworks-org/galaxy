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

	return RUN_ALL_TESTS();
}
