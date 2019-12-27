///
/// KeyUpEvent.hpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#ifndef CELESTIAL_KEYUPEVENT_HPP_
#define CELESTIAL_KEYUPEVENT_HPP_

///
/// Core namespace.
///
namespace celestial
{
	///
	/// Contains the key that was released.
	///
	struct KeyUpEvent final
	{
		///
		/// Default constructor.
		///
		KeyUpEvent() noexcept;

		///
		/// Constructor.
		///
		/// \param keycode Keycode of key pressed.
		///
		KeyUpEvent(const int keycode) noexcept;

		///
		/// Default destructor.
		///
		~KeyUpEvent() = default;

		///
		/// Keycode for the key that was pressed.
		///
		int m_keycode;
	};
}

#endif