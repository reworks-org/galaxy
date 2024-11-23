///
/// Mouse.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Mouse.hpp"

namespace galaxy
{
	namespace input
	{
		namespace mouse
		{
			bool is_mouse_button_pressed(const ray::MouseButton button) noexcept
			{
				return ::IsMouseButtonPressed(static_cast<int>(button));
			}

			bool is_mouse_button_released(const ray::MouseButton button) noexcept
			{
				return ::IsMouseButtonReleased(static_cast<int>(button));
			}

			bool is_mouse_button_down(const ray::MouseButton button) noexcept
			{
				return ::IsMouseButtonDown(static_cast<int>(button));
			}

			bool is_mouse_button_up(const ray::MouseButton button) noexcept
			{
				return ::IsMouseButtonUp(static_cast<int>(button));
			}

			int get_mouse_x() noexcept
			{
				return ::GetMouseX();
			}

			int get_mouse_y() noexcept
			{
				return ::GetMouseY();
			}

			math::Vector2 get_mouse_position() noexcept
			{
				return {GetMousePosition()};
			}

			math::Vector2 get_mouse_delta() noexcept
			{
				return {GetMouseDelta()};
			}

			float get_mouse_wheel_move() noexcept
			{
				return ::GetMouseWheelMove();
			}

			math::Vector2 get_mouse_wheel_move_v() noexcept
			{
				return {GetMouseWheelMoveV()};
			}
		} // namespace mouse
	} // namespace input
} // namespace galaxy
