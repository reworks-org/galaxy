///
/// KeyUpEvent.hpp
/// starlight
///
/// Created by reworks on 21/12/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_KEYUPEVENT_HPP_
#define STARLIGHT_KEYUPEVENT_HPP_

namespace sl
{
	///
	/// Contains the key that was released.
	///
	struct KeyUpEvent final
	{
		///
		/// Constructor.
		///
		inline KeyUpEvent(const int keycode)
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