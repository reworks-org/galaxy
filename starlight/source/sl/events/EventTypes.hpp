///
/// EventTypes.hpp
/// starlight
///
/// Created by reworks on 15/01/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_EVENTTYPES_HPP_
#define STARLIGHT_EVENTTYPES_HPP_

#include <allegro5/events.h>

namespace sl
{
	///
	/// \brief A data structure of event types.
	///
	/// Can be inherited and you can add your own types.
	/// BASE_EVENT is what we use of offset the rest of the events. Don't use it.
	/// New event id's are calculated as BASE_EVENT + n, where n = 1, 2, etc. DO NOT USE + 0, that is the same as BASE_EVENT!
	///
	struct EventTypes
	{
		///
		/// The base event value all other events must start from.
		///
		static constexpr unsigned int BASE_EVENT = ALLEGRO_GET_EVENT_TYPE('S', 'T', 'A', 'R');

		///
		/// Collision event identifier.
		///
		static constexpr unsigned int COLLISION_EVENT = BASE_EVENT + 1;

		///
		/// Particle emit event identifier.
		///
		static constexpr unsigned int PARTICLE_EMIT_EVENT = BASE_EVENT + 2;

		///
		/// Player movement event identifier.
		///
		static constexpr unsigned int PLAYER_MOVE_EVENT = BASE_EVENT + 3;
	};

	///
	/// Allows the usage of EventType to make code more understandable.
	///
	typedef unsigned int EventType;
}

#endif