///
/// MouseButtons.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_INPUT_MOUSEBUTTONS_HPP_
#define GALAXY_INPUT_MOUSEBUTTONS_HPP_

namespace galaxy
{
	namespace input
	{
		///
		/// \brief Enum class representing mouse buttons.
		///
		/// Values used are based off of GLFW.
		///
		enum class MouseButtons : int
		{
			UNKNOWN    = -1,
			BTN_1      = 0,
			BTN_2      = 1,
			BTN_3      = 2,
			BTN_4      = 3,
			BTN_5      = 4,
			BTN_6      = 5,
			BTN_7      = 6,
			BTN_8      = 7,
			BTN_LAST   = BTN_8,
			BTN_LEFT   = BTN_1,
			BTN_RIGHT  = BTN_2,
			BTN_MIDDLE = BTN_3
		};
	} // namespace input
} // namespace galaxy

#endif
