///
/// main.cpp
///
/// solar
/// See LICENSE.txt.
///

#include "gtest/gtest.h"
#include <solar/entity/Manager.hpp>

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);
	auto result = RUN_ALL_TESTS();
	
	std::cin.get();

	return result;
}