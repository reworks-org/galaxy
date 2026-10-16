/** @file
 * @brief Implements SDL event polling and immediate input state queries.
 */

#include "input.h"

#include "display.h"

#include <SDL2/SDL.h>

namespace sl
{

	bool key_down(SDL_Scancode key)
	{
		SDL_PumpEvents();
		const Uint8 *state = SDL_GetKeyboardState(nullptr);
		return state && key >= 0 && key < SDL_NUM_SCANCODES && state[key] != 0;
	}
	bool key_down(Event::Key key)
	{
		SDL_Scancode sdl_key=SDL_SCANCODE_UNKNOWN;
		switch(key)
		{
			case Event::Key::letter_a:
			sdl_key=SDL_SCANCODE_A;
			break;
			case Event::Key::letter_b:
			sdl_key=SDL_SCANCODE_B;
			break;
			case Event::Key::letter_c:
			sdl_key=SDL_SCANCODE_C;
			break;
			case Event::Key::letter_d:
			sdl_key=SDL_SCANCODE_D;
			break;
			case Event::Key::letter_e:
			sdl_key=SDL_SCANCODE_E;
			break;
			case Event::Key::letter_f:
			sdl_key=SDL_SCANCODE_F;
			break;
			case Event::Key::letter_g:
			sdl_key=SDL_SCANCODE_G;
			break;
			case Event::Key::letter_h:
			sdl_key=SDL_SCANCODE_H;
			break;
			case Event::Key::letter_i:
			sdl_key=SDL_SCANCODE_I;
			break;
			case Event::Key::letter_j:
			sdl_key=SDL_SCANCODE_J;
			break;
			case Event::Key::letter_k:
			sdl_key=SDL_SCANCODE_K;
			break;
			case Event::Key::letter_l:
			sdl_key=SDL_SCANCODE_L;
			break;
			case Event::Key::letter_m:
			sdl_key=SDL_SCANCODE_M;
			break;
			case Event::Key::letter_n:
			sdl_key=SDL_SCANCODE_N;
			break;
			case Event::Key::letter_o:
			sdl_key=SDL_SCANCODE_O;
			break;
			case Event::Key::letter_p:
			sdl_key=SDL_SCANCODE_P;
			break;
			case Event::Key::letter_q:
			sdl_key=SDL_SCANCODE_Q;
			break;
			case Event::Key::letter_r:
			sdl_key=SDL_SCANCODE_R;
			break;
			case Event::Key::letter_s:
			sdl_key=SDL_SCANCODE_S;
			break;
			case Event::Key::letter_t:
			sdl_key=SDL_SCANCODE_T;
			break;
			case Event::Key::letter_u:
			sdl_key=SDL_SCANCODE_U;
			break;
			case Event::Key::letter_v:
			sdl_key=SDL_SCANCODE_V;
			break;
			case Event::Key::letter_w:
			sdl_key=SDL_SCANCODE_W;
			break;
			case Event::Key::letter_x:
			sdl_key=SDL_SCANCODE_X;
			break;
			case Event::Key::letter_y:
			sdl_key=SDL_SCANCODE_Y;
			break;
			case Event::Key::letter_z:
			sdl_key=SDL_SCANCODE_Z;
			break;
			case Event::Key::comma:
			sdl_key=SDL_SCANCODE_COMMA;
			break;
			case Event::Key::f1:
			sdl_key=SDL_SCANCODE_F1;
			break;
			case Event::Key::f2:
			sdl_key=SDL_SCANCODE_F2;
			break;
			case Event::Key::f3:
			sdl_key=SDL_SCANCODE_F3;
			break;
			case Event::Key::f4:
			sdl_key=SDL_SCANCODE_F4;
			break;
			case Event::Key::f5:
			sdl_key=SDL_SCANCODE_F5;
			break;
			case Event::Key::f6:
			sdl_key=SDL_SCANCODE_F6;
			break;
			case Event::Key::f7:
			sdl_key=SDL_SCANCODE_F7;
			break;
			case Event::Key::f8:
			sdl_key=SDL_SCANCODE_F8;
			break;
			case Event::Key::f9:
			sdl_key=SDL_SCANCODE_F9;
			break;
			case Event::Key::f10:
			sdl_key=SDL_SCANCODE_F10;
			break;
			case Event::Key::f11:
			sdl_key=SDL_SCANCODE_F11;
			break;
			case Event::Key::f12:
			sdl_key=SDL_SCANCODE_F12;
			break;
			case Event::Key::digit_0:
			sdl_key=SDL_SCANCODE_0;
			break;
			case Event::Key::digit_1:
			sdl_key=SDL_SCANCODE_1;
			break;
			case Event::Key::digit_2:
			sdl_key=SDL_SCANCODE_2;
			break;
			case Event::Key::digit_3:
			sdl_key=SDL_SCANCODE_3;
			break;
			case Event::Key::digit_4:
			sdl_key=SDL_SCANCODE_4;
			break;
			case Event::Key::digit_5:
			sdl_key=SDL_SCANCODE_5;
			break;
			case Event::Key::digit_6:
			sdl_key=SDL_SCANCODE_6;
			break;
			case Event::Key::digit_7:
			sdl_key=SDL_SCANCODE_7;
			break;
			case Event::Key::digit_8:
			sdl_key=SDL_SCANCODE_8;
			break;
			case Event::Key::digit_9:
			sdl_key=SDL_SCANCODE_9;
			break;
			case Event::Key::space:
			sdl_key=SDL_SCANCODE_SPACE;
			break;
			case Event::Key::tab:
			sdl_key=SDL_SCANCODE_TAB;
			break;
			case Event::Key::return_key:
			sdl_key=SDL_SCANCODE_RETURN;
			break;
			case Event::Key::keypad_enter:
			sdl_key=SDL_SCANCODE_KP_ENTER;
			break;
			case Event::Key::backspace:
			sdl_key=SDL_SCANCODE_BACKSPACE;
			break;
			case Event::Key::delete_key:
			sdl_key=SDL_SCANCODE_DELETE;
			break;
			case Event::Key::insert:
			sdl_key=SDL_SCANCODE_INSERT;
			break;
			case Event::Key::home:
			sdl_key=SDL_SCANCODE_HOME;
			break;
			case Event::Key::end:
			sdl_key=SDL_SCANCODE_END;
			break;
			case Event::Key::page_up:
			sdl_key=SDL_SCANCODE_PAGEUP;
			break;
			case Event::Key::page_down:
			sdl_key=SDL_SCANCODE_PAGEDOWN;
			break;
			case Event::Key::left_shift:
			sdl_key=SDL_SCANCODE_LSHIFT;
			break;
			case Event::Key::right_shift:
			sdl_key=SDL_SCANCODE_RSHIFT;
			break;
			case Event::Key::left_control:
			sdl_key=SDL_SCANCODE_LCTRL;
			break;
			case Event::Key::right_control:
			sdl_key=SDL_SCANCODE_RCTRL;
			break;
			case Event::Key::left_alt:
			sdl_key=SDL_SCANCODE_LALT;
			break;
			case Event::Key::right_alt:
			sdl_key=SDL_SCANCODE_RALT;
			break;
			case Event::Key::left_gui:
			sdl_key=SDL_SCANCODE_LGUI;
			break;
			case Event::Key::right_gui:
			sdl_key=SDL_SCANCODE_RGUI;
			break;
			case Event::Key::caps_lock:
			sdl_key=SDL_SCANCODE_CAPSLOCK;
			break;
			case Event::Key::num_lock:
			sdl_key=SDL_SCANCODE_NUMLOCKCLEAR;
			break;
			case Event::Key::scroll_lock:
			sdl_key=SDL_SCANCODE_SCROLLLOCK;
			break;
			case Event::Key::pause:
			sdl_key=SDL_SCANCODE_PAUSE;
			break;
			case Event::Key::print_screen:
			sdl_key=SDL_SCANCODE_PRINTSCREEN;
			break;
			case Event::Key::minus:
			sdl_key=SDL_SCANCODE_MINUS;
			break;
			case Event::Key::equals:
			sdl_key=SDL_SCANCODE_EQUALS;
			break;
			case Event::Key::left_bracket:
			sdl_key=SDL_SCANCODE_LEFTBRACKET;
			break;
			case Event::Key::right_bracket:
			sdl_key=SDL_SCANCODE_RIGHTBRACKET;
			break;
			case Event::Key::backslash:
			sdl_key=SDL_SCANCODE_BACKSLASH;
			break;
			case Event::Key::semicolon:
			sdl_key=SDL_SCANCODE_SEMICOLON;
			break;
			case Event::Key::period:
			sdl_key=SDL_SCANCODE_PERIOD;
			break;
			case Event::Key::slash:
			sdl_key=SDL_SCANCODE_SLASH;
			break;
			case Event::Key::backquote:
			sdl_key=SDL_SCANCODE_GRAVE;
			break;
			case Event::Key::escape:
			sdl_key=SDL_SCANCODE_ESCAPE;
			break;
			case Event::Key::arrow_right:
			sdl_key=SDL_SCANCODE_RIGHT;
			break;
			case Event::Key::arrow_left:
			sdl_key=SDL_SCANCODE_LEFT;
			break;
			case Event::Key::arrow_down:
			sdl_key=SDL_SCANCODE_DOWN;
			break;
			case Event::Key::arrow_up:
			sdl_key=SDL_SCANCODE_UP;
			break;
			default:
			break;
		}
		
		return key_down(sdl_key);
	}
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
	} // namespace

	int mouse_x()
	{
		int x = 0;
		SDL_GetMouseState(&x, nullptr);
		int window_w = 0;
		int window_h = 0;
		if (SDL_Window *window = get_window())
		{
			SDL_GetWindowSize(window, &window_w, &window_h);
		}
		return scale_to_logical(x, window_w, virtual_screen_width());
	}

	int mouse_y()
	{
		int y = 0;
		SDL_GetMouseState(nullptr, &y);
		int window_w = 0;
		int window_h = 0;
		if (SDL_Window *window = get_window())
		{
			SDL_GetWindowSize(window, &window_w, &window_h);
		}
		return scale_to_logical(y, window_h, virtual_screen_height());
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

	bool set_relative_mouse_mode(bool enabled)
	{
		return SDL_SetRelativeMouseMode(enabled ? SDL_TRUE : SDL_FALSE) == 0;
	}

	bool relative_mouse_mode()
	{
		return SDL_GetRelativeMouseMode() == SDL_TRUE;
	}

	std::uint32_t mouse_relative_state(int *dx, int *dy)
	{
		int rx = 0;
		int ry = 0;
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