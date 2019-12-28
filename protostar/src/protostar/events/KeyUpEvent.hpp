///
/// KeyUpEvent.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_KEYUPEVENT_HPP_
#define PROTOSTAR_KEYUPEVENT_HPP_

///
/// Core namespace.
///
namespace protostar
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