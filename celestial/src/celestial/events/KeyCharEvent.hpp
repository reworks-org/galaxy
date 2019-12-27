///
/// KeyCharEvent.hpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#ifndef CELESTIAL_KEYCHAREVENT_HPP_
#define CELESTIAL_KEYCHAREVENT_HPP_

///
/// Core namespace.
///
namespace celestial
{
	///
	/// Event for use with unicode characters, not KeyDown, etc...
	///
	struct KeyCharEvent final
	{
		///
		/// Default constructor.
		///
		KeyCharEvent() noexcept;

		///
		/// Argument constructor.
		///
		/// \param keycode Code of the key pressed.
		/// \param unichar Unicode character pressed.
		///
		KeyCharEvent(const int keycode, const int unichar) noexcept;

		///
		/// Default destructor.
		///
		~KeyCharEvent() = default;

		///
		/// Keycode for the key that was pressed.
		///
		int m_keycode;

		///
		/// Unicode character for use with unicode strings.
		///
		int m_unichar;
	};
}

#endif