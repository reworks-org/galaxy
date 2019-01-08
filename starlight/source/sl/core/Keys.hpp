///
/// Keys.hpp
/// starlight
///
/// Created by reworks on 21/12/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_KEYS_HPP_
#define STARLIGHT_KEYS_HPP_

namespace sl
{
	///
	/// \brief Keys configured for this game engine.
	///
	/// You need to set these to correspond to allegro 5's keycodes. See here: https://liballeg.org/a5docs/trunk/keyboard.html#key-codes
	/// They are all default initialized to 0.
	///
	struct Keys final
	{
		///
		/// Commonly used to move the player forward.
		///
		static inline int KEY_FORWARD = 0;
		
		///
		/// Commonly used to move the player backward.
		///
		static inline int KEY_BACKWARD = 0;

		///
		/// Commonly used to move the player left.
		///
		static inline int KEY_LEFT = 0;

		///
		/// Commonly used to move the player right.
		///
		static inline int KEY_RIGHT = 0;

		///
		/// Commonly used to confirm something in the ui.
		///
		static inline int UI_CONFIRM = 0;

		///
		/// Commonly used to move an indicator left in the ui.
		///
		static inline int UI_LEFT = 0;

		///
		/// Commonly used to move an indicator right in the ui.
		///
		static inline int UI_RIGHT = 0;
	};
}

#endif