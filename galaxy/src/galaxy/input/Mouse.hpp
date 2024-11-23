///
/// Mouse.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_INPUT_MOUSE_HPP_
#define GALAXY_INPUT_MOUSE_HPP_

#include "galaxy/math/Vector2.hpp"

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
			bool is_mouse_button_pressed(const ray::MouseButton button) noexcept;

			///
			/// Check if a mouse button has been released once.
			///
			/// \param button Mouse button.
			///
			/// \return Boolean.
			///
			[[nodiscard]]
			bool is_mouse_button_released(const ray::MouseButton button) noexcept;

			///
			/// Check if a mouse button is being pressed.
			///
			/// \param button Mouse button.
			///
			/// \return Boolean.
			///
			[[nodiscard]]
			bool is_mouse_button_down(const ray::MouseButton button) noexcept;

			///
			/// Check if a mouse button is NOT being pressed.
			///
			/// \param button Mouse button.
			///
			/// \return Boolean.
			///
			[[nodiscard]]
			bool is_mouse_button_up(const ray::MouseButton button) noexcept;

			///
			/// Get mouse position X.
			///
			/// \return Int.
			///
			[[nodiscard]]
			int get_mouse_x() noexcept;

			///
			//// Get mouse position Y.
			///
			/// \return Int.
			///
			[[nodiscard]]
			int get_mouse_y() noexcept;

			///
			/// Get mouse position XY.
			///
			/// \return Vec2.
			///
			[[nodiscard]]
			math::Vector2 get_mouse_position() noexcept;

			///
			/// Get mouse delta between frames.
			///
			/// \return Vec2.
			///
			[[nodiscard]]
			math::Vector2 get_mouse_delta() noexcept;

			///
			/// Get mouse wheel movement for X or Y, whichever is larger.
			///
			/// \return Float.
			///
			[[nodiscard]]
			float get_mouse_wheel_move() noexcept;

			///
			/// Get mouse wheel movement for both X and Y.
			///
			/// \return Vec2.
			///
			[[nodiscard]]
			math::Vector2 get_mouse_wheel_move_v() noexcept;
		} // namespace mouse
	} // namespace input
} // namespace galaxy

#endif
