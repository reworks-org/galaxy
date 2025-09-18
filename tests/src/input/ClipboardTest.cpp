///
/// ClipboardTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <SDL3/SDL.h>

#include <gtest/gtest.h>

#include <galaxy/input/Clipboard.hpp>

TEST(Clipboard, SetGet)
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
	std::string input = "test";

	galaxy::input::set_clipboard(input);
	std::string get = galaxy::input::get_clipboard();

	ASSERT_TRUE(get == input);

	SDL_Quit();
}
