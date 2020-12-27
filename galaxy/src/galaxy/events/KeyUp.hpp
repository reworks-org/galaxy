///
/// KeyUp.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_KEYUP_HPP_
#define GALAXY_EVENTS_KEYUP_HPP_

#include "galaxy/input/Keys.hpp"

namespace galaxy
{
	namespace events
	{
		///
		/// Contains the key that was released.
		///
		struct KeyUp final
		{
			///
			/// Default constructor.
			///
			KeyUp() = default;

			///
			/// Constructor.
			///
			/// \param keycode Keycode of key pressed.
			///
			KeyUp(const input::Keys keycode);

			///
			/// Default destructor.
			///
			~KeyUp() = default;

			///
			/// Keycode for the key that was pressed.
			///
			input::Keys m_keycode;
		};
	} // namespace events
} // namespace galaxy

#endif