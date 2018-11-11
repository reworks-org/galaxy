///
/// main.cpp
/// Tests
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include <SDL2/SDL_main.h>

#include "googletest/gtest/gtest.h"

int main(int argsc, char *argsv[])
{
	testing::InitGoogleTest(&argsc, argsv);
	
	return RUN_ALL_TESTS();
}