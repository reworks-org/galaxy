///
/// main.cpp
///
/// pulsar
/// See LICENSE.txt.
///

#include "gtest/gtest.h"
#include <pl/Log.hpp>

void testfunc()
{
	pl::Log::init("logs/a.txt");
	LOG_S(WARNING) << "hmm";
}

int main(int argc, char** argv)
{
	//testing::InitGoogleTest(&argc, argv);

	//auto result = RUN_ALL_TESTS();
	
	testfunc();

	std::cin.get();
	
	return 0;
	//return result;
}