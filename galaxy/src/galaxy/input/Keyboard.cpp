///
/// Keyboard.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <GLFW/glfw3.h>

#include "Keyboard.hpp"

namespace galaxy
{
	namespace input
	{
		Keyboard::Keyboard() noexcept
			: m_text_input {""}
			, m_inputting_text {false}
		{
			m_keymap.reserve(102);
			m_keymap.emplace(Keys::A, GLFW_KEY_A);
			m_keymap.emplace(Keys::B, GLFW_KEY_B);
			m_keymap.emplace(Keys::C, GLFW_KEY_C);
			m_keymap.emplace(Keys::D, GLFW_KEY_D);
			m_keymap.emplace(Keys::E, GLFW_KEY_E);
			m_keymap.emplace(Keys::F, GLFW_KEY_F);
			m_keymap.emplace(Keys::G, GLFW_KEY_G);
			m_keymap.emplace(Keys::H, GLFW_KEY_H);
			m_keymap.emplace(Keys::I, GLFW_KEY_I);
			m_keymap.emplace(Keys::J, GLFW_KEY_J);
			m_keymap.emplace(Keys::K, GLFW_KEY_K);
			m_keymap.emplace(Keys::L, GLFW_KEY_L);
			m_keymap.emplace(Keys::M, GLFW_KEY_M);
			m_keymap.emplace(Keys::N, GLFW_KEY_N);
			m_keymap.emplace(Keys::O, GLFW_KEY_O);
			m_keymap.emplace(Keys::P, GLFW_KEY_P);
			m_keymap.emplace(Keys::Q, GLFW_KEY_Q);
			m_keymap.emplace(Keys::R, GLFW_KEY_R);
			m_keymap.emplace(Keys::S, GLFW_KEY_S);
			m_keymap.emplace(Keys::T, GLFW_KEY_T);
			m_keymap.emplace(Keys::U, GLFW_KEY_U);
			m_keymap.emplace(Keys::V, GLFW_KEY_V);
			m_keymap.emplace(Keys::W, GLFW_KEY_W);
			m_keymap.emplace(Keys::X, GLFW_KEY_X);
			m_keymap.emplace(Keys::Y, GLFW_KEY_Y);
			m_keymap.emplace(Keys::Z, GLFW_KEY_Z);
			m_keymap.emplace(Keys::KEY_1, GLFW_KEY_1);
			m_keymap.emplace(Keys::KEY_2, GLFW_KEY_2);
			m_keymap.emplace(Keys::KEY_3, GLFW_KEY_3);
			m_keymap.emplace(Keys::KEY_4, GLFW_KEY_4);
			m_keymap.emplace(Keys::KEY_5, GLFW_KEY_5);
			m_keymap.emplace(Keys::KEY_6, GLFW_KEY_6);
			m_keymap.emplace(Keys::KEY_7, GLFW_KEY_7);
			m_keymap.emplace(Keys::KEY_8, GLFW_KEY_8);
			m_keymap.emplace(Keys::KEY_9, GLFW_KEY_9);
			m_keymap.emplace(Keys::KEY_0, GLFW_KEY_0);
			m_keymap.emplace(Keys::MINUS, GLFW_KEY_MINUS);
			m_keymap.emplace(Keys::EQUALS, GLFW_KEY_EQUAL);
			m_keymap.emplace(Keys::BACKSPACE, GLFW_KEY_BACKSPACE);
			m_keymap.emplace(Keys::GRAVE, GLFW_KEY_GRAVE_ACCENT);
			m_keymap.emplace(Keys::TAB, GLFW_KEY_TAB);
			m_keymap.emplace(Keys::CAPS, GLFW_KEY_CAPS_LOCK);
			m_keymap.emplace(Keys::LSHIFT, GLFW_KEY_LEFT_SHIFT);
			m_keymap.emplace(Keys::LCNTRL, GLFW_KEY_LEFT_CONTROL);
			m_keymap.emplace(Keys::LSTART, GLFW_KEY_LEFT_SUPER);
			m_keymap.emplace(Keys::LALT, GLFW_KEY_LEFT_ALT);
			m_keymap.emplace(Keys::SPACE, GLFW_KEY_SPACE);
			m_keymap.emplace(Keys::RALT, GLFW_KEY_RIGHT_ALT);
			m_keymap.emplace(Keys::RSTART, GLFW_KEY_RIGHT_SUPER);
			m_keymap.emplace(Keys::MENU, GLFW_KEY_MENU);
			m_keymap.emplace(Keys::RCNTRL, GLFW_KEY_RIGHT_CONTROL);
			m_keymap.emplace(Keys::RSHIFT, GLFW_KEY_RIGHT_SHIFT);
			m_keymap.emplace(Keys::ENTER, GLFW_KEY_ENTER);
			m_keymap.emplace(Keys::SEMICOLON, GLFW_KEY_SEMICOLON);
			m_keymap.emplace(Keys::APOSTROPHE, GLFW_KEY_APOSTROPHE);
			m_keymap.emplace(Keys::SLASH, GLFW_KEY_SLASH);
			m_keymap.emplace(Keys::PERIOD, GLFW_KEY_PERIOD);
			m_keymap.emplace(Keys::COMMA, GLFW_KEY_COMMA);
			m_keymap.emplace(Keys::LBRACKET, GLFW_KEY_LEFT_BRACKET);
			m_keymap.emplace(Keys::RBRACKET, GLFW_KEY_RIGHT_BRACKET);
			m_keymap.emplace(Keys::BACKSLASH, GLFW_KEY_BACKSLASH);
			m_keymap.emplace(Keys::ESC, GLFW_KEY_ESCAPE);
			m_keymap.emplace(Keys::F1, GLFW_KEY_F1);
			m_keymap.emplace(Keys::F2, GLFW_KEY_F2);
			m_keymap.emplace(Keys::F3, GLFW_KEY_F3);
			m_keymap.emplace(Keys::F4, GLFW_KEY_F4);
			m_keymap.emplace(Keys::F5, GLFW_KEY_F5);
			m_keymap.emplace(Keys::F6, GLFW_KEY_F6);
			m_keymap.emplace(Keys::F7, GLFW_KEY_F7);
			m_keymap.emplace(Keys::F8, GLFW_KEY_F8);
			m_keymap.emplace(Keys::F9, GLFW_KEY_F9);
			m_keymap.emplace(Keys::F10, GLFW_KEY_F10);
			m_keymap.emplace(Keys::F11, GLFW_KEY_F11);
			m_keymap.emplace(Keys::F12, GLFW_KEY_F12);
			m_keymap.emplace(Keys::PRINTSCREEN, GLFW_KEY_PRINT_SCREEN);
			m_keymap.emplace(Keys::SCROLL_LOCK, GLFW_KEY_SCROLL_LOCK);
			m_keymap.emplace(Keys::PAUSE, GLFW_KEY_PAUSE);
			m_keymap.emplace(Keys::INSERT, GLFW_KEY_INSERT);
			m_keymap.emplace(Keys::HOME, GLFW_KEY_HOME);
			m_keymap.emplace(Keys::PAGEUP, GLFW_KEY_PAGE_UP);
			m_keymap.emplace(Keys::PAGEDOWN, GLFW_KEY_PAGE_DOWN);
			m_keymap.emplace(Keys::END, GLFW_KEY_END);
			m_keymap.emplace(Keys::DEL, GLFW_KEY_DELETE);
			m_keymap.emplace(Keys::UP, GLFW_KEY_UP);
			m_keymap.emplace(Keys::DOWN, GLFW_KEY_DOWN);
			m_keymap.emplace(Keys::LEFT, GLFW_KEY_LEFT);
			m_keymap.emplace(Keys::RIGHT, GLFW_KEY_RIGHT);
			m_keymap.emplace(Keys::NUMLOCK, GLFW_KEY_NUM_LOCK);
			m_keymap.emplace(Keys::NUMPAD_DIVIDE, GLFW_KEY_KP_DIVIDE);
			m_keymap.emplace(Keys::NUMPAD_MULTIPLY, GLFW_KEY_KP_MULTIPLY);
			m_keymap.emplace(Keys::NUMPAD_ADD, GLFW_KEY_KP_ADD);
			m_keymap.emplace(Keys::NUMPAD_ENTER, GLFW_KEY_KP_ENTER);
			m_keymap.emplace(Keys::NUMPAD_PERIOD, GLFW_KEY_KP_DECIMAL);
			m_keymap.emplace(Keys::NUMPAD_0, GLFW_KEY_KP_0);
			m_keymap.emplace(Keys::NUMPAD_1, GLFW_KEY_KP_1);
			m_keymap.emplace(Keys::NUMPAD_2, GLFW_KEY_KP_2);
			m_keymap.emplace(Keys::NUMPAD_3, GLFW_KEY_KP_3);
			m_keymap.emplace(Keys::NUMPAD_4, GLFW_KEY_KP_4);
			m_keymap.emplace(Keys::NUMPAD_5, GLFW_KEY_KP_5);
			m_keymap.emplace(Keys::NUMPAD_6, GLFW_KEY_KP_6);
			m_keymap.emplace(Keys::NUMPAD_7, GLFW_KEY_KP_7);
			m_keymap.emplace(Keys::NUMPAD_8, GLFW_KEY_KP_8);
			m_keymap.emplace(Keys::NUMPAD_9, GLFW_KEY_KP_9);

			m_reverse_keymap.reserve(102);
			for (const auto& [key, value] : m_keymap)
			{
				m_reverse_keymap.emplace(value, key);
			}

			m_prev_key_states.reserve(102);
			m_prev_key_states.emplace(Keys::A, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::B, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::C, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::D, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::E, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::F, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::G, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::H, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::I, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::J, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::K, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::L, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::M, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::N, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::O, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::P, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::Q, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::R, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::S, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::T, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::U, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::V, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::W, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::X, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::Y, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::Z, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::KEY_1, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::KEY_2, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::KEY_3, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::KEY_4, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::KEY_5, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::KEY_6, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::KEY_7, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::KEY_8, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::KEY_9, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::KEY_0, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::MINUS, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::EQUALS, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::BACKSPACE, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::GRAVE, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::TAB, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::CAPS, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::LSHIFT, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::LCNTRL, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::LSTART, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::LALT, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::SPACE, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::RALT, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::RSTART, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::MENU, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::RCNTRL, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::RSHIFT, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::ENTER, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::SEMICOLON, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::APOSTROPHE, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::SLASH, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::PERIOD, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::COMMA, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::LBRACKET, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::RBRACKET, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::BACKSLASH, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::ESC, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::F1, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::F2, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::F3, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::F4, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::F5, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::F6, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::F7, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::F8, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::F9, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::F10, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::F11, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::F12, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::PRINTSCREEN, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::SCROLL_LOCK, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::PAUSE, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::INSERT, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::HOME, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::PAGEUP, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::PAGEDOWN, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::END, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::DEL, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::UP, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::DOWN, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::LEFT, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::RIGHT, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::NUMLOCK, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::NUMPAD_MULTIPLY, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::NUMPAD_DIVIDE, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::NUMPAD_ADD, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::NUMPAD_ENTER, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::NUMPAD_PERIOD, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::NUMPAD_0, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::NUMPAD_1, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::NUMPAD_2, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::NUMPAD_3, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::NUMPAD_4, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::NUMPAD_5, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::NUMPAD_6, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::NUMPAD_7, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::NUMPAD_8, GLFW_RELEASE);
			m_prev_key_states.emplace(Keys::NUMPAD_9, GLFW_RELEASE);
		}

		Keyboard::Keyboard(Keyboard&& k) noexcept
		{
			this->m_inputting_text  = std::move(k.m_inputting_text);
			this->m_keymap          = std::move(k.m_keymap);
			this->m_prev_key_states = std::move(k.m_prev_key_states);
			this->m_reverse_keymap  = std::move(k.m_reverse_keymap);
			this->m_text_input      = std::move(k.m_text_input);

			k.m_text_input.clear();
		}

		Keyboard& Keyboard::operator=(Keyboard&& k) noexcept
		{
			if (this != &k)
			{
				this->m_inputting_text  = std::move(k.m_inputting_text);
				this->m_keymap          = std::move(k.m_keymap);
				this->m_prev_key_states = std::move(k.m_prev_key_states);
				this->m_reverse_keymap  = std::move(k.m_reverse_keymap);
				this->m_text_input      = std::move(k.m_text_input);

				k.m_text_input.clear();
			}

			return *this;
		}

		Keyboard::Keyboard(const Keyboard& k) noexcept
		{
			this->m_inputting_text  = k.m_inputting_text;
			this->m_keymap          = k.m_keymap;
			this->m_prev_key_states = k.m_prev_key_states;
			this->m_reverse_keymap  = k.m_reverse_keymap;
			this->m_text_input      = k.m_text_input;
		}

		Keyboard& Keyboard::operator=(const Keyboard& k) noexcept
		{
			if (this != &k)
			{
				this->m_inputting_text  = k.m_inputting_text;
				this->m_keymap          = k.m_keymap;
				this->m_prev_key_states = k.m_prev_key_states;
				this->m_reverse_keymap  = k.m_reverse_keymap;
				this->m_text_input      = k.m_text_input;
			}

			return *this;
		}

		Keyboard::~Keyboard() noexcept
		{
			m_keymap.clear();
			m_reverse_keymap.clear();
			m_prev_key_states.clear();
			m_text_input.clear();
		}
	} // namespace input
} // namespace galaxy