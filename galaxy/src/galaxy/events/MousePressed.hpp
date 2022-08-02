///
/// MousePressed.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_MOUSEPRESSED_HPP_
#define GALAXY_EVENTS_MOUSEPRESSED_HPP_

#include "galaxy/input/InputMods.hpp"
#include "galaxy/input/MouseButtons.hpp"

namespace galaxy
{
	namespace events
	{
		///
		/// Contains data relating to a mouse pressed event.
		///
		struct MousePressed final
		{
			///
			/// Mouse x position.
			///
			double m_xpos;

			///
			/// Mouse y position.
			///
			double m_ypos;

			///
			/// Mouse button pressed.
			///
			input::MouseButtons m_button;

			///
			/// Modifier Key.
			///
			input::InputMods m_mod;
		};
	} // namespace events
} // namespace galaxy

#endif