///
/// KeyCharEvent.hpp
/// galaxy
///
/// Created by reworks on 21/12/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_KEYCHAREVENT_HPP_
#define GALAXY_KEYCHAREVENT_HPP_

namespace galaxy
{
	///
	/// Used when allegro fires a key char event, which contains unicode data.
	/// Don't use this if your just checking key presses. Use this when you need unicode characters.
	///
	struct KeyCharEvent final
	{
		///
		/// Constructor.
		///
		inline KeyCharEvent(const int keycode, const int unichar)
			:m_keycode(keycode), m_unichar(unichar)
		{
		}

		///
		/// Allegro keycode for the key that was pressed.
		///
		int m_keycode = 0;

		///
		/// Unicode character for use with Allegro's unicode strings.
		///
		int m_unichar = 0;
	};
}

#endif