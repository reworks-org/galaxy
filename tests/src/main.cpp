///
/// main.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>
#include <pulsar/Log.hpp>

int main(int argsc, char* argsv[])
{
	PL_ENABLE_TESTING_MODE;

	testing::InitGoogleTest(&argsc, argsv);
	auto result = RUN_ALL_TESTS();

	std::cin.get();

	return result;
}