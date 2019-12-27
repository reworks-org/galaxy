///
/// KeyDownEvent.hpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#ifndef CELESTIAL_KEYDOWNEVENT_HPP_
#define CELESTIAL_KEYDOWNEVENT_HPP_

///
/// Core namespace.
///
namespace celestial
{
	///
	/// Contains the key that was pressed down.
	///
	struct KeyDownEvent final
	{
		///
		/// Default constructor.
		///
		KeyDownEvent() noexcept;

		///
		/// Constructor.
		///
		/// \param keycode Keycode for the key that was pressed.
		///
		KeyDownEvent(const int keycode) noexcept;

		///
		/// Default destructor.
		///
		~KeyDownEvent() = default;	

		///
		/// Keycode for the key that was pressed.
		///
		int m_keycode;
	};
}

#endif