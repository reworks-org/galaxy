/** @file
 * @brief Implements SDL event polling and immediate input state queries.
 */

#include "input.h"

#include "display.h"

#include <SDL3/SDL.h>

#include <cmath>

namespace sl
{
	namespace
	{
		/** Scale a raw SDL window coordinate into the logical screen coordinate space used for drawing. */
		int scale_to_logical(int value, int window_size, int logical_size)
		{
			if (window_size <= 0 || logical_size <= 0)
			{
				return value;
			}
			return value * logical_size / window_size;
		}
		int mapping_x = 0, mapping_y = 0;
		float mapping_scale_x = 1.0f, mapping_scale_y = 1.0f;
		bool mapping_active = false;
	} // namespace

	void set_mouse_mapping(int x, int y, float scale_x, float scale_y)
	{
		if (!(scale_x > 0.0f) || !(scale_y > 0.0f))
			return;
		mapping_x = x;
		mapping_y = y;
		mapping_scale_x = scale_x;
		mapping_scale_y = scale_y;
		mapping_active = true;
	}

	void clear_mouse_mapping()
	{
		mapping_active = false;
	}

	int mouse_x()
	{
		float x = 0;
		SDL_GetMouseState(&x, nullptr);
		int window_w = 0;
		int window_h = 0;
		if (SDL_Window *window = get_window())
		{
			SDL_GetWindowSize(window, &window_w, &window_h);
		}
		const int screen_x = scale_to_logical(x, window_w, virtual_screen_width());
		return mapping_active ? static_cast<int>(std::floor((screen_x - mapping_x) / mapping_scale_x)) : screen_x;
	}

	int mouse_y()
	{
		float y = 0;
		SDL_GetMouseState(nullptr, &y);
		int window_w = 0;
		int window_h = 0;
		if (SDL_Window *window = get_window())
		{
			SDL_GetWindowSize(window, &window_w, &window_h);
		}
		const int screen_y = scale_to_logical(y, window_h, virtual_screen_height());
		return mapping_active ? static_cast<int>(std::floor((screen_y - mapping_y) / mapping_scale_y)) : screen_y;
	}

	std::uint32_t mouse_buttons()
	{
		return SDL_GetMouseState(nullptr, nullptr);
	}

	bool mouse_button_down(MouseButton button)
	{
		const int b = static_cast<int>(button);
		if (b < 1 || b > 31)
		{
			return false;
		}
		return (mouse_buttons() & (1u << (b - 1))) != 0;
	}

	bool mouse_button_down(int button)
	{
		if (button < 1 || button > 31)
		{
			return false;
		}
		return (mouse_buttons() & (1u << (button - 1))) != 0;
	}

	bool set_relative_mouse_mode(SDL_Window* window, bool enabled)
	{
		return SDL_SetWindowRelativeMouseMode(window, enabled);
	}

	bool relative_mouse_mode(SDL_Window* window)
	{
		return SDL_GetWindowRelativeMouseMode(window);
	}

	std::uint32_t mouse_relative_state(int *dx, int *dy)
	{
		float rx = 0;
		float ry = 0;
		const std::uint32_t buttons = SDL_GetRelativeMouseState(&rx, &ry);
		if (dx)
		{
			*dx = rx;
		}
		if (dy)
		{
			*dy = ry;
		}
		return buttons;
	}

	void mouse_delta(int *dx, int *dy)
	{
		mouse_relative_state(dx, dy);
	}

} // namespace sl