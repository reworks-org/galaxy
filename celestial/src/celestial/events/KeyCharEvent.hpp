///
/// KeyCharEvent.hpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#ifndef CELESTIAL_KEYCHAREVENT_HPP_
#define CELESTIAL_KEYCHAREVENT_HPP_

namespace celestial
{
	///
	/// Event for use with unicode characters, not KeyDown, etc...
	///
	struct KeyCharEvent final
	{
		///
		/// Constructor.
		///
		/// \param keycode Code of the key pressed.
		/// \param unichar Unicode character pressed.
		///
		inline KeyCharEvent(const int keycode, const int unichar) noexcept
			:m_keycode(keycode), m_unichar(unichar)
		{
		}

		///
		/// Keycode for the key that was pressed.
		///
		int m_keycode = 0;

		///
		/// Unicode character for use with unicode strings.
		///
		int m_unichar = 0;
	};
}

#endif