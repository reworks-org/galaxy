///
/// Mouse.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_INPUT_MOUSE_HPP_
#define GALAXY_INPUT_MOUSE_HPP_

#include <raylib.h>

namespace galaxy
{
	namespace input
	{
		namespace mouse
		{
			///
			/// Check if a mouse button has been pressed once.
			///
			/// \param button Mouse button.
			///
			/// \return Boolean.
			///
			[[nodiscard]]
			bool is_mouse_button_pressed(const MouseButton button);

			///
			/// Check if a mouse button has been released once.
			///
			/// \param button Mouse button.
			///
			/// \return Boolean.
			///
			[[nodiscard]]
			bool is_mouse_button_released(const MouseButton button);

			///
			/// Check if a mouse button is being pressed.
			///
			/// \param button Mouse button.
			///
			/// \return Boolean.
			///
			[[nodiscard]]
			bool is_mouse_button_down(const MouseButton button);

			///
			/// Check if a mouse button is NOT being pressed.
			///
			/// \param button Mouse button.
			///
			/// \return Boolean.
			///
			[[nodiscard]]
			bool is_mouse_button_up(const MouseButton button);

			///
			/// Get mouse position X.
			///
			/// \return Int.
			///
			[[nodiscard]]
			int get_mouse_x();

			///
			//// Get mouse position Y.
			///
			/// \return Int.
			///
			[[nodiscard]]
			int get_mouse_y();

			///
			/// Get mouse position XY.
			///
			/// \return Vec2.
			///
			[[nodiscard]]
			Vector2 get_mouse_position();

			///
			/// Get mouse delta between frames.
			///
			/// \return Vec2.
			///
			[[nodiscard]]
			Vector2 get_mouse_delta();

			///
			/// Get mouse wheel movement for X or Y, whichever is larger.
			///
			/// \return Float.
			///
			[[nodiscard]]
			float get_mouse_wheel_move();

			///
			/// Get mouse wheel movement for both X and Y.
			///
			/// \return Vec2.
			///
			[[nodiscard]]
			Vector2 get_mouse_wheel_move_v();
		} // namespace mouse
	} // namespace input
} // namespace galaxy

#endif
