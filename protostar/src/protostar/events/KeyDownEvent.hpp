///
/// KeyDownEvent.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_KEYDOWNEVENT_HPP_
#define PROTOSTAR_KEYDOWNEVENT_HPP_

#include <protostar/system/Keys.hpp>

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
		KeyDownEvent() noexcept = default;

		///
		/// Constructor.
		///
		/// \param keycode Keycode for the key that was pressed.
		///
		KeyDownEvent(const protostar::Keys keycode) noexcept;

		///
		/// Default destructor.
		///
		~KeyDownEvent() = default;	

		///
		/// Keycode for the key that was pressed.
		///
		protostar::Keys m_keycode;
	};
}

#endif