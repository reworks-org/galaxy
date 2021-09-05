///
/// Keys.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_INPUT_KEYS_HPP_
#define GALAXY_INPUT_KEYS_HPP_

namespace galaxy
{
	namespace input
	{
		///
		/// Enum class representing keys.
		///
		enum class Keys : int
		{
			UNDEFINED = -1,
			A,
			B,
			C,
			D,
			E,
			F,
			G,
			H,
			I,
			J,
			K,
			L,
			M,
			N,
			O,
			P,
			Q,
			R,
			S,
			T,
			U,
			V,
			W,
			X,
			Y,
			Z,
			KEY_1,
			KEY_2,
			KEY_3,
			KEY_4,
			KEY_5,
			KEY_6,
			KEY_7,
			KEY_8,
			KEY_9,
			KEY_0,
			MINUS,
			EQUALS,
			BACKSPACE,
			GRAVE,
			TAB,
			CAPS,
			LSHIFT,
			LCNTRL,
			LSTART,
			LALT,
			SPACE,
			RALT,
			RSTART,
			MENU,
			RCNTRL,
			RSHIFT,
			ENTER,
			SEMICOLON,
			APOSTROPHE,
			SLASH,
			PERIOD,
			COMMA,
			LBRACKET,
			RBRACKET,
			BACKSLASH,
			ESC,
			F1,
			F2,
			F3,
			F4,
			F5,
			F6,
			F7,
			F8,
			F9,
			F10,
			F11,
			F12,
			PRINTSCREEN,
			SCROLL_LOCK,
			PAUSE,
			INSERT,
			HOME,
			PAGEUP,
			PAGEDOWN,
			END,
			DEL,
			UP,
			DOWN,
			LEFT,
			RIGHT,
			NUMLOCK,
			NUMPAD_MULTIPLY,
			NUMPAD_DIVIDE,
			NUMPAD_ADD,
			NUMPAD_ENTER,
			NUMPAD_PERIOD,
			NUMPAD_0,
			NUMPAD_1,
			NUMPAD_2,
			NUMPAD_3,
			NUMPAD_4,
			NUMPAD_5,
			NUMPAD_6,
			NUMPAD_7,
			NUMPAD_8,
			NUMPAD_9
		};

		///
		/// \brief Modifier key mappings.
		///
		/// Values taken from GLFW source code.
		///
		enum class KeyMod : int
		{
			///
			/// If this bit is set one or more Shift keys were held down.
			///
			SHIFT = 0x0001,

			///
			/// If this bit is set one or more Control keys were held down.
			///
			CONTROL = 0x0002,

			///
			/// If this bit is set one or more Alt keys were held down.
			///
			ALT = 0x0004,

			///
			/// If this bit is set one or more Super keys were held down.
			///
			SUPER = 0x0008,

			///
			/// If this bit is set then the Caps Lock key is enabled.
			///
			CAPS_LOCK = 0x0010,

			///
			/// If this bit is set then the Num Lock key is enabled.
			///
			NUM_LOCK = 0x0020
		};
	} // namespace input
} // namespace galaxy

#endif