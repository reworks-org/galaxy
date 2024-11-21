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
			bool is_mouse_button_pressed(const MouseButton button)
			{
				return IsMouseButtonPressed(static_cast<int>(button));
			}

			bool is_mouse_button_released(const MouseButton button)
			{
				return IsMouseButtonReleased(static_cast<int>(button));
			}

			bool is_mouse_button_down(const MouseButton button)
			{
				return IsMouseButtonDown(static_cast<int>(button));
			}

			bool is_mouse_button_up(const MouseButton button)
			{
				return IsMouseButtonUp(static_cast<int>(button));
			}

			int get_mouse_x()
			{
				return GetMouseX();
			}

			int get_mouse_y()
			{
				return GetMouseY();
			}

			Vector2 get_mouse_position()
			{
				return GetMousePosition();
			}

			Vector2 get_mouse_delta()
			{
				return GetMouseDelta();
			}

			float get_mouse_wheel_move()
			{
				return GetMouseWheelMove();
			}

			Vector2 get_mouse_wheel_move_v()
			{
				return GetMouseWheelMoveV();
			}
		} // namespace mouse
	} // namespace input
} // namespace galaxy
