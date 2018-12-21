///
/// KeyDownEvent.hpp
/// starlight
///
/// Created by reworks on 21/12/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_KEYDOWNEVENT_HPP_
#define STARLIGHT_KEYDOWNEVENT_HPP_

namespace sl
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