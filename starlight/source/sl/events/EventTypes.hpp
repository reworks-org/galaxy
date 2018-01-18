///
/// EventTypes.hpp
/// starlight
///
///  Created by reworks on 15/01/2018.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_EVENTTYPES_HPP_
#define STARLIGHT_EVENTTYPES_HPP_

#include <allegro5/events.h>

///
/// \brief A data structure of event types.
///
/// Can be inherited and you can add your own types.
/// BASE_EVENT is what we use of offset the rest of the events. Don't use it.
/// New event id's are calculated as BASE_EVENT + n, where n = 1, 2, etc. DO NOT USE + 0, that is the same as BASE_EVENT!
///
struct EventTypes
{
	static constexpr int BASE_EVENT = ALLEGRO_GET_EVENT_TYPE('S', 'T', 'A', 'R') + ALLEGRO_GET_EVENT_TYPE('L', 'I', 'T', 'E');
	static constexpr int COLLISION_EVENT = BASE_EVENT + 1;
	static constexpr int PARTICLE_EMIT_EVENT = BASE_EVENT + 2;
};

typedef int EventType;

#endif