///
/// RMLInput.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "RMLInput.hpp"

namespace galaxy
{
	namespace ui
	{
		RMLInput& RMLInput::handle() noexcept
		{
			static RMLInput input;
			return input;
		}

		RMLInput::RMLInput() noexcept
		{
			m_keymap.reserve(102);
			m_keymap.emplace(input::Keys::A, Rml::Input::KeyIdentifier::KI_A);
			m_keymap.emplace(input::Keys::B, Rml::Input::KeyIdentifier::KI_B);
			m_keymap.emplace(input::Keys::C, Rml::Input::KeyIdentifier::KI_C);
			m_keymap.emplace(input::Keys::D, Rml::Input::KeyIdentifier::KI_D);
			m_keymap.emplace(input::Keys::E, Rml::Input::KeyIdentifier::KI_E);
			m_keymap.emplace(input::Keys::F, Rml::Input::KeyIdentifier::KI_F);
			m_keymap.emplace(input::Keys::G, Rml::Input::KeyIdentifier::KI_G);
			m_keymap.emplace(input::Keys::H, Rml::Input::KeyIdentifier::KI_H);
			m_keymap.emplace(input::Keys::I, Rml::Input::KeyIdentifier::KI_I);
			m_keymap.emplace(input::Keys::J, Rml::Input::KeyIdentifier::KI_J);
			m_keymap.emplace(input::Keys::K, Rml::Input::KeyIdentifier::KI_K);
			m_keymap.emplace(input::Keys::L, Rml::Input::KeyIdentifier::KI_L);
			m_keymap.emplace(input::Keys::M, Rml::Input::KeyIdentifier::KI_M);
			m_keymap.emplace(input::Keys::N, Rml::Input::KeyIdentifier::KI_N);
			m_keymap.emplace(input::Keys::O, Rml::Input::KeyIdentifier::KI_O);
			m_keymap.emplace(input::Keys::P, Rml::Input::KeyIdentifier::KI_P);
			m_keymap.emplace(input::Keys::Q, Rml::Input::KeyIdentifier::KI_Q);
			m_keymap.emplace(input::Keys::R, Rml::Input::KeyIdentifier::KI_R);
			m_keymap.emplace(input::Keys::S, Rml::Input::KeyIdentifier::KI_S);
			m_keymap.emplace(input::Keys::T, Rml::Input::KeyIdentifier::KI_T);
			m_keymap.emplace(input::Keys::U, Rml::Input::KeyIdentifier::KI_U);
			m_keymap.emplace(input::Keys::V, Rml::Input::KeyIdentifier::KI_V);
			m_keymap.emplace(input::Keys::W, Rml::Input::KeyIdentifier::KI_W);
			m_keymap.emplace(input::Keys::X, Rml::Input::KeyIdentifier::KI_X);
			m_keymap.emplace(input::Keys::Y, Rml::Input::KeyIdentifier::KI_Y);
			m_keymap.emplace(input::Keys::Z, Rml::Input::KeyIdentifier::KI_Z);
			m_keymap.emplace(input::Keys::KEY_1, Rml::Input::KeyIdentifier::KI_1);
			m_keymap.emplace(input::Keys::KEY_2, Rml::Input::KeyIdentifier::KI_2);
			m_keymap.emplace(input::Keys::KEY_3, Rml::Input::KeyIdentifier::KI_3);
			m_keymap.emplace(input::Keys::KEY_4, Rml::Input::KeyIdentifier::KI_4);
			m_keymap.emplace(input::Keys::KEY_5, Rml::Input::KeyIdentifier::KI_5);
			m_keymap.emplace(input::Keys::KEY_6, Rml::Input::KeyIdentifier::KI_6);
			m_keymap.emplace(input::Keys::KEY_7, Rml::Input::KeyIdentifier::KI_7);
			m_keymap.emplace(input::Keys::KEY_8, Rml::Input::KeyIdentifier::KI_8);
			m_keymap.emplace(input::Keys::KEY_9, Rml::Input::KeyIdentifier::KI_9);
			m_keymap.emplace(input::Keys::KEY_0, Rml::Input::KeyIdentifier::KI_0);
			m_keymap.emplace(input::Keys::MINUS, Rml::Input::KeyIdentifier::KI_OEM_MINUS);
			m_keymap.emplace(input::Keys::EQUALS, Rml::Input::KeyIdentifier::KI_OEM_PLUS);
			m_keymap.emplace(input::Keys::BACKSPACE, Rml::Input::KeyIdentifier::KI_BACK);
			m_keymap.emplace(input::Keys::GRAVE, Rml::Input::KeyIdentifier::KI_OEM_3);
			m_keymap.emplace(input::Keys::TAB, Rml::Input::KeyIdentifier::KI_TAB);
			m_keymap.emplace(input::Keys::CAPS, Rml::Input::KeyIdentifier::KI_CAPITAL);
			m_keymap.emplace(input::Keys::SPACE, Rml::Input::KeyIdentifier::KI_SPACE);
			m_keymap.emplace(input::Keys::MENU, Rml::Input::KeyIdentifier::KI_APPS);
			m_keymap.emplace(input::Keys::LSHIFT, Rml::Input::KeyIdentifier::KI_LSHIFT);
			m_keymap.emplace(input::Keys::LCNTRL, Rml::Input::KeyIdentifier::KI_LCONTROL);
			m_keymap.emplace(input::Keys::RCNTRL, Rml::Input::KeyIdentifier::KI_RCONTROL);
			m_keymap.emplace(input::Keys::RSHIFT, Rml::Input::KeyIdentifier::KI_RSHIFT);
			//m_keymap.emplace(input::Keys::LALT, Rml::Input::KeyIdentifier::);
			//m_keymap.emplace(input::Keys::RALT, Rml::Input::KeyIdentifier::);
			//m_keymap.emplace(input::Keys::LSTART, Rml::Input::KeyIdentifier::);
			//m_keymap.emplace(input::Keys::RSTART, Rml::Input::KeyIdentifier::);
			m_keymap.emplace(input::Keys::ENTER, Rml::Input::KeyIdentifier::KI_RETURN);
			m_keymap.emplace(input::Keys::SEMICOLON, Rml::Input::KeyIdentifier::KI_OEM_1);
			m_keymap.emplace(input::Keys::APOSTROPHE, Rml::Input::KeyIdentifier::KI_OEM_7);
			m_keymap.emplace(input::Keys::SLASH, Rml::Input::KeyIdentifier::KI_OEM_2);
			m_keymap.emplace(input::Keys::PERIOD, Rml::Input::KeyIdentifier::KI_OEM_PERIOD);
			m_keymap.emplace(input::Keys::COMMA, Rml::Input::KeyIdentifier::KI_OEM_COMMA);
			m_keymap.emplace(input::Keys::LBRACKET, Rml::Input::KeyIdentifier::KI_OEM_4);
			m_keymap.emplace(input::Keys::RBRACKET, Rml::Input::KeyIdentifier::KI_OEM_6);
			m_keymap.emplace(input::Keys::BACKSLASH, Rml::Input::KeyIdentifier::KI_OEM_5);
			m_keymap.emplace(input::Keys::ESC, Rml::Input::KeyIdentifier::KI_ESCAPE);
			m_keymap.emplace(input::Keys::F1, Rml::Input::KeyIdentifier::KI_F1);
			m_keymap.emplace(input::Keys::F2, Rml::Input::KeyIdentifier::KI_F2);
			m_keymap.emplace(input::Keys::F3, Rml::Input::KeyIdentifier::KI_F3);
			m_keymap.emplace(input::Keys::F4, Rml::Input::KeyIdentifier::KI_F4);
			m_keymap.emplace(input::Keys::F5, Rml::Input::KeyIdentifier::KI_F5);
			m_keymap.emplace(input::Keys::F6, Rml::Input::KeyIdentifier::KI_F6);
			m_keymap.emplace(input::Keys::F7, Rml::Input::KeyIdentifier::KI_F7);
			m_keymap.emplace(input::Keys::F8, Rml::Input::KeyIdentifier::KI_F8);
			m_keymap.emplace(input::Keys::F9, Rml::Input::KeyIdentifier::KI_F9);
			m_keymap.emplace(input::Keys::F10, Rml::Input::KeyIdentifier::KI_F10);
			m_keymap.emplace(input::Keys::F11, Rml::Input::KeyIdentifier::KI_F11);
			m_keymap.emplace(input::Keys::F12, Rml::Input::KeyIdentifier::KI_F12);
			m_keymap.emplace(input::Keys::PRINTSCREEN, Rml::Input::KeyIdentifier::KI_SNAPSHOT);
			m_keymap.emplace(input::Keys::SCROLL_LOCK, Rml::Input::KeyIdentifier::KI_SCROLL);
			m_keymap.emplace(input::Keys::PAUSE, Rml::Input::KeyIdentifier::KI_PAUSE);
			m_keymap.emplace(input::Keys::INSERT, Rml::Input::KeyIdentifier::KI_INSERT);
			m_keymap.emplace(input::Keys::HOME, Rml::Input::KeyIdentifier::KI_HOME);
			m_keymap.emplace(input::Keys::PAGEUP, Rml::Input::KeyIdentifier::KI_PRIOR);
			m_keymap.emplace(input::Keys::PAGEDOWN, Rml::Input::KeyIdentifier::KI_NEXT);
			m_keymap.emplace(input::Keys::END, Rml::Input::KeyIdentifier::KI_END);
			m_keymap.emplace(input::Keys::DEL, Rml::Input::KeyIdentifier::KI_DELETE);
			m_keymap.emplace(input::Keys::UP, Rml::Input::KeyIdentifier::KI_UP);
			m_keymap.emplace(input::Keys::DOWN, Rml::Input::KeyIdentifier::KI_DOWN);
			m_keymap.emplace(input::Keys::LEFT, Rml::Input::KeyIdentifier::KI_LEFT);
			m_keymap.emplace(input::Keys::RIGHT, Rml::Input::KeyIdentifier::KI_RIGHT);
			m_keymap.emplace(input::Keys::NUMLOCK, Rml::Input::KeyIdentifier::KI_NUMLOCK);
			m_keymap.emplace(input::Keys::NUMPAD_DIVIDE, Rml::Input::KeyIdentifier::KI_DIVIDE);
			m_keymap.emplace(input::Keys::NUMPAD_MULTIPLY, Rml::Input::KeyIdentifier::KI_MULTIPLY);
			m_keymap.emplace(input::Keys::NUMPAD_ADD, Rml::Input::KeyIdentifier::KI_ADD);
			m_keymap.emplace(input::Keys::NUMPAD_ENTER, Rml::Input::KeyIdentifier::KI_NUMPADENTER);
			m_keymap.emplace(input::Keys::NUMPAD_PERIOD, Rml::Input::KeyIdentifier::KI_DECIMAL);
			m_keymap.emplace(input::Keys::NUMPAD_0, Rml::Input::KeyIdentifier::KI_NUMPAD0);
			m_keymap.emplace(input::Keys::NUMPAD_1, Rml::Input::KeyIdentifier::KI_NUMPAD1);
			m_keymap.emplace(input::Keys::NUMPAD_2, Rml::Input::KeyIdentifier::KI_NUMPAD2);
			m_keymap.emplace(input::Keys::NUMPAD_3, Rml::Input::KeyIdentifier::KI_NUMPAD3);
			m_keymap.emplace(input::Keys::NUMPAD_4, Rml::Input::KeyIdentifier::KI_NUMPAD4);
			m_keymap.emplace(input::Keys::NUMPAD_5, Rml::Input::KeyIdentifier::KI_NUMPAD5);
			m_keymap.emplace(input::Keys::NUMPAD_6, Rml::Input::KeyIdentifier::KI_NUMPAD6);
			m_keymap.emplace(input::Keys::NUMPAD_7, Rml::Input::KeyIdentifier::KI_NUMPAD7);
			m_keymap.emplace(input::Keys::NUMPAD_8, Rml::Input::KeyIdentifier::KI_NUMPAD8);
			m_keymap.emplace(input::Keys::NUMPAD_9, Rml::Input::KeyIdentifier::KI_NUMPAD9);

			m_modmap.reserve(6);
			m_modmap.emplace(input::KeyMod::SHIFT, Rml::Input::KeyModifier::KM_SHIFT);
			m_modmap.emplace(input::KeyMod::CONTROL, Rml::Input::KeyModifier::KM_CTRL);
			m_modmap.emplace(input::KeyMod::ALT, Rml::Input::KeyModifier::KM_ALT);
			m_modmap.emplace(input::KeyMod::SUPER, Rml::Input::KeyModifier::KM_META);
			m_modmap.emplace(input::KeyMod::CAPS_LOCK, Rml::Input::KeyModifier::KM_CAPSLOCK);
			m_modmap.emplace(input::KeyMod::NUM_LOCK, Rml::Input::KeyModifier::KM_NUMLOCK);
		}

		RMLInput::~RMLInput() noexcept
		{
			m_keymap.clear();
			m_modmap.clear();
		}
	} // namespace ui
} // namespace galaxy