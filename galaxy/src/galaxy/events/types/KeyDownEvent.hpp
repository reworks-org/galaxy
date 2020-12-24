///
/// KeyDownEvent.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_KEYDOWNEVENT_HPP_
#define PROTOSTAR_KEYDOWNEVENT_HPP_

#include "protostar/system/Keys.hpp"

///
/// Core namespace.
///
namespace pr
{
	///
	/// Contains the key that was pressed down.
	///
	struct KeyDownEvent final
	{
		///
		/// Default constructor.
		///
		KeyDownEvent() = default;

		///
		/// Constructor.
		///
		/// \param keycode Keycode for the key that was pressed.
		///
		KeyDownEvent(const pr::Keys keycode);

		///
		/// Copy constructor.
		///
		KeyDownEvent(const KeyDownEvent&) = default;

		///
		/// Move constructor.
		///
		KeyDownEvent(KeyDownEvent&&) = default;

		///
		/// Copy assignment operator.
		///
		KeyDownEvent& operator=(const KeyDownEvent&) = default;

		///
		/// Move assignment operator.
		///
		KeyDownEvent& operator=(KeyDownEvent&&) = default;

		///
		/// Default destructor.
		///
		~KeyDownEvent() = default;

		///
		/// Keycode for the key that was pressed.
		///
		pr::Keys m_keycode;
	};
} // namespace pr

#endif