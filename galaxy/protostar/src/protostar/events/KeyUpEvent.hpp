///
/// KeyUpEvent.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_KEYUPEVENT_HPP_
#define PROTOSTAR_KEYUPEVENT_HPP_

#include "protostar/system/Keys.hpp"

///
/// Core namespace.
///
namespace pr
{
	///
	/// Contains the key that was released.
	///
	struct KeyUpEvent final
	{
		///
		/// Default constructor.
		///
		KeyUpEvent() = default;

		///
		/// Constructor.
		///
		/// \param keycode Keycode of key pressed.
		///
		KeyUpEvent(const pr::Keys keycode);

		///
		/// Copy constructor.
		///
		KeyUpEvent(const KeyUpEvent&) = default;

		///
		/// Move constructor.
		///
		KeyUpEvent(KeyUpEvent&&) = default;

		///
		/// Copy assignment operator.
		///
		KeyUpEvent& operator=(const KeyUpEvent&) = default;

		///
		/// Move assignment operator.
		///
		KeyUpEvent& operator=(KeyUpEvent&&) = default;

		///
		/// Default destructor.
		///
		~KeyUpEvent() = default;

		///
		/// Keycode for the key that was pressed.
		///
		pr::Keys m_keycode;
	};
} // namespace pr

#endif