///
/// KeyUpEvent.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_KEYUPEVENT_HPP_
#define PROTOSTAR_KEYUPEVENT_HPP_

#include <protostar/system/Keys.hpp>

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
		KeyUpEvent() noexcept = default;

		///
		/// Constructor.
		///
		/// \param keycode Keycode of key pressed.
		///
		KeyUpEvent(const protostar::Keys keycode) noexcept;

		///
		/// Default destructor.
		///
		~KeyUpEvent() = default;

		///
		/// Keycode for the key that was pressed.
		///
		protostar::Keys m_keycode;
	};
}

#endif