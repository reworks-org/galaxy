///
/// JoystickDisconnect.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_JOYSTICKDISCONNECT_HPP_
#define GALAXY_EVENTS_JOYSTICKDISCONNECT_HPP_

namespace galaxy
{
	namespace events
	{
		///
		/// POD tag to allow a function to accept a dispatched focus lost event.
		///
		struct JoystickDisconnect
		{
			///
			/// Index of the joystick (in range [0 .. Joystick::Count - 1]).
			///
			unsigned int id;
		};
	} // namespace events
} // namespace galaxy

#endif
