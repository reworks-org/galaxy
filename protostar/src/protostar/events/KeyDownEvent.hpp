///
/// KeyDownEvent.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_KEYDOWNEVENT_HPP_
#define PROTOSTAR_KEYDOWNEVENT_HPP_

///
/// Core namespace.
///
namespace protostar
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