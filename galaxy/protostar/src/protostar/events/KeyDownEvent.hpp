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
		KeyDownEvent() noexcept = default;

		///
		/// Constructor.
		///
		/// \param keycode Keycode for the key that was pressed.
		///
		KeyDownEvent(const pr::Keys keycode) noexcept;

		///
		/// Copy constructor.
		///
		KeyDownEvent(const KeyDownEvent&) noexcept = default;

		///
		/// Move constructor.
		///
		KeyDownEvent(KeyDownEvent&&) noexcept = default;

		///
		/// Copy assignment operator.
		///
		KeyDownEvent& operator=(const KeyDownEvent&) noexcept = default;

		///
		/// Move assignment operator.
		///
		KeyDownEvent& operator=(KeyDownEvent&&) noexcept = default;

		///
		/// Default destructor.
		///
		~KeyDownEvent() noexcept = default;

		///
		/// Keycode for the key that was pressed.
		///
		pr::Keys m_keycode;
	};
} // namespace pr

#endif