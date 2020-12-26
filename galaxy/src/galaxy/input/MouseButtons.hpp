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
	} // namespace input
} // namespace galaxy

#endif