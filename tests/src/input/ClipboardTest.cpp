///
/// ClipboardTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>

#include <galaxy/input/Clipboard.hpp>

TEST(Clipboard, SetGet)
{
	auto input = "test";

	galaxy::input::set_clipboard(input);
	auto get = galaxy::input::get_clipboard();

	ASSERT_TRUE(get == input);
}

TEST(Clipboard, EmptyGet)
{
	auto get = galaxy::input::get_clipboard();

	ASSERT_TRUE(get.empty());
}
