///
/// KeyUpEvent.hpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#ifndef CELESTIAL_KEYUPEVENT_HPP_
#define CELESTIAL_KEYUPEVENT_HPP_

namespace celestial
{
	///
	/// Contains the key that was released.
	///
	struct KeyUpEvent final
	{
		///
		/// Constructor.
		///
		/// \param keycode Keycode of key pressed.
		///
		inline KeyUpEvent(const int keycode) noexcept
			:m_keycode(keycode)
		{
		}

		///
		/// Keycode for the key that was pressed.
		///
		int m_keycode = 0;
	};
}

#endif