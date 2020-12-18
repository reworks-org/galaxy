///
/// MouseButtons.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_MOUSEBUTTONS_HPP_
#define PROTOSTAR_MOUSEBUTTONS_HPP_

///
/// Core namespace.
///
namespace pr
{
	///
	/// Enum class representing keys.
	///
	enum class MouseButton : int
	{
		BUTTON_1,
		BUTTON_2,
		BUTTON_3,
		BUTTON_4,
		BUTTON_5,
		BUTTON_6,
		BUTTON_7,
		BUTTON_8,
		BUTTON_LAST,
		BUTTON_LEFT,
		BUTTON_RIGHT,
		BUTTON_MIDDLE
	};
} // namespace pr

#endif