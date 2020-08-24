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
		KeyUpEvent() noexcept = default;

		///
		/// Constructor.
		///
		/// \param keycode Keycode of key pressed.
		///
		KeyUpEvent(const pr::Keys keycode) noexcept;

		///
		/// Copy constructor.
		///
		KeyUpEvent(const KeyUpEvent&) noexcept = default;

		///
		/// Move constructor.
		///
		KeyUpEvent(KeyUpEvent&&) noexcept = default;

		///
		/// Copy assignment operator.
		///
		KeyUpEvent& operator=(const KeyUpEvent&) noexcept = default;

		///
		/// Move assignment operator.
		///
		KeyUpEvent& operator=(KeyUpEvent&&) noexcept = default;

		///
		/// Default destructor.
		///
		~KeyUpEvent() noexcept = default;

		///
		/// Keycode for the key that was pressed.
		///
		pr::Keys m_keycode;
	};
} // namespace pr

#endif