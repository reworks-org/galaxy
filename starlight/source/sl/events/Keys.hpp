///
/// Keys.hpp
/// starlight
///
/// Created by reworks on 20/01/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_KEYS_HPP_
#define STARLIGHT_KEYS_HPP_

namespace sl
{
	///
	/// \brief Use this data structure to configure and check keybinds.
	///
	/// You need to assign values to these keys, otherwise they all default to 0.
	/// The value you set must be a valid ALLEGRO keycode.
	/// See: https://liballeg.org/a5docs/5.2.4/keyboard.html#key-codes
	/// Or "allegro5/keycodes.h"
	///
	struct Keys
	{
		///
		/// Key to represent forward movement.
		///
		static inline unsigned int KEY_FORWARD = 0;

		///
		/// Key to represent backward movement.
		///
		static inline unsigned int KEY_BACKWARD = 0;

		///
		/// Key to represent left movement.
		///
		static inline unsigned int KEY_LEFT = 0;

		///
		/// Key to represent right movement.
		///
		static inline unsigned int KEY_RIGHT = 0;

		///
		/// Represents a key that when pressed closes the application.
		///
		static inline unsigned int KEY_QUIT = 0;
	};
}

#endif