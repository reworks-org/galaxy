///
/// main.cpp
///
/// solar
/// See LICENSE.txt.
///

#include "gtest/gtest.h"

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);

	RUN_ALL_TESTS();

	std::cin.get();

	return EXIT_SUCCESS;
}