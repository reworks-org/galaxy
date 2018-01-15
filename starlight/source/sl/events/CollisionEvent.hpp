///
/// CollisionEvent.hpp
/// starlight
///
///  Created by reworks on 15/01/2018.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_COLLISIONEVENT_HPP_
#define STARLIGHT_COLLISIONEVENT_HPP_

#include "entt/entity/registry.hpp"

struct CollisionEvent
{
	/// Entity A in collision.
	entt::Entity a;

	/// Entity B in collision.
	entt::Entity b;

	/// Optional amount of particles to display.
	int particle_count = 0;
};

#endif