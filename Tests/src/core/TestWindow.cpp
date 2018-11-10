///
/// TestWindow.cpp
/// Quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include <qs/core/Window.hpp>

#include "gtest/gtest.h"

TEST(Window, DefaultConstructor)
{
	// Make sure default params are set correctly.
	qs::Window window;

	ASSERT_EQ(true, window.isOpen());
	ASSERT_EQ(nullptr, window.getWindow());
	ASSERT_EQ(nullptr, window.getContext());

	// Now test creation and destruction.
	bool result = window.create("test", 800, 600, SDL_WINDOW_HIDDEN);

	ASSERT_EQ(true, result);
	ASSERT_NE(nullptr, window.getWindow());
	ASSERT_NE(nullptr, window.getContext());

	window.destroy();
	ASSERT_EQ(nullptr, window.getWindow());
	ASSERT_EQ(nullptr, window.getContext());
}

TEST(Window, AlternateConstructor)
{
	// Make sure it is properly constructed from constructor and that it is properly destroyed.
	qs::Window window("test", 800, 600, SDL_WINDOW_HIDDEN);

	ASSERT_EQ(true, window.isOpen());
	ASSERT_NE(nullptr, window.getWindow());
	ASSERT_NE(nullptr, window.getContext());

	window.destroy();
	ASSERT_EQ(nullptr, window.getWindow());
	ASSERT_EQ(nullptr, window.getContext());
}