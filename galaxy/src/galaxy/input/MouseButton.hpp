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
	enum class MouseButton : int
	{
		LEFT   = SDL_BUTTON_LEFT,
		MIDDLE = SDL_BUTTON_MIDDLE,
		RIGHT  = SDL_BUTTON_RIGHT,
		SIDE_1 = SDL_BUTTON_X1,
		SIDE_2 = SDL_BUTTON_X2
	};

	///
	/// Enum class for mouse wheel direction.
	///
	enum class MouseWheelDirection : int
	{
		NORMAL  = static_cast<int>(SDL_MouseWheelDirection::SDL_MOUSEWHEEL_NORMAL),
		FLIPPED = static_cast<int>(SDL_MouseWheelDirection::SDL_MOUSEWHEEL_FLIPPED)
	};

	namespace input
	{
		///
		/// Converts an int to a mouse button.
		///
		/// \param mouse Integer mouse button.
		///
		/// \return Enum mouse button.
		///
		[[nodiscard]]
		inline constexpr MouseButton int_to_mouse_btn(const int mouse)
		{
			return static_cast<MouseButton>(mouse);
		}

		///
		/// Converts a mouse button to an int.
		///
		/// \param mouse Enum mouse button.
		///
		/// \return Integer mouse button.
		///
		[[nodiscard]]
		inline constexpr int mouse_btn_to_int(const MouseButton mouse)
		{
			return static_cast<int>(mouse);
		}

		///
		/// Converts an int to a mouse wheel direction.
		///
		/// \param direction Integer wheel direction.
		///
		/// \return Enum mouse wheel direction.
		///
		[[nodiscard]]
		inline constexpr MouseWheelDirection int_to_mousewheel_dir(const int direction)
		{
			return static_cast<MouseWheelDirection>(direction);
		}

		///
		/// Converts an SDL wheel direction to a mouse wheel direction.
		///
		/// \param direction SDL wheel direction.
		///
		/// \return Enum mouse wheel direction.
		///
		[[nodiscard]]
		inline constexpr MouseWheelDirection int_to_mousewheel_dir(const SDL_MouseWheelDirection direction)
		{
			const auto as_int = static_cast<int>(direction);
			return int_to_mousewheel_dir(as_int);
		}

		///
		/// Converts a mouse wheel direction to an int.
		///
		/// \param direction Enum mouse wheel direction.
		///
		/// \return Integer wheel direction.
		///
		[[nodiscard]]
		inline constexpr int mousewheel_to_int(const MouseWheelDirection direction)
		{
			return static_cast<int>(direction);
		}
	} // namespace input
} // namespace galaxy

#endif
