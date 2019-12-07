///
/// KeyDownEvent.hpp
/// galaxy
///
/// Created by reworks on 21/12/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_KEYDOWNEVENT_HPP_
#define GALAXY_KEYDOWNEVENT_HPP_

namespace galaxy
{
	///
	/// Contains the key that was pressed down.
	///
	struct KeyDownEvent final
	{
		///
		/// Constructor.
		///
		inline KeyDownEvent(const int keycode)
			:m_keycode(keycode)
		{
		}

		///
		/// Allegro keycode for the key that was pressed.
		///
		int m_keycode = 0;
	};
}

#endif