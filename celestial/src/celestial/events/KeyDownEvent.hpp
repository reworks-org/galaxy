///
/// KeyDownEvent.hpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#ifndef CELESTIAL_KEYDOWNEVENT_HPP_
#define CELESTIAL_KEYDOWNEVENT_HPP_

namespace celestial
{
	///
	/// Contains the key that was pressed down.
	///
	struct KeyDownEvent final
	{
		///
		/// Constructor.
		///
		/// \param keycode Keycode for the key that was pressed.
		///
		inline KeyDownEvent(const int keycode) noexcept
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