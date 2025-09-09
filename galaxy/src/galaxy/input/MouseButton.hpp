///
/// MouseButton.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_INPUT_MOUSEBUTTON_HPP_
#define GALAXY_INPUT_MOUSEBUTTON_HPP_

#include <SDL3/SDL_mouse.h>

namespace galaxy
{
	///
	/// Enum class representing mouse buttons.
	///
	enum class MouseButton : SDL_MouseButtonFlags
	{
		UNKNOWN = 0,
		LEFT    = SDL_BUTTON_LEFT,
		MIDDLE  = SDL_BUTTON_MIDDLE,
		RIGHT   = SDL_BUTTON_RIGHT,
		SIDE_1  = SDL_BUTTON_X1,
		SIDE_2  = SDL_BUTTON_X2
	};
} // namespace galaxy

#endif
