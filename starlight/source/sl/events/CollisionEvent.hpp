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

struct CollisionEvent
{
	/// Entity A in collision.
	/// We use the non typedef of entt::Entity because it saves us having to include a bunch of headers...
	unsigned int a;

	/// Entity B in collision.
	unsigned int b;

	/// Optional amount of particles to display.
	int particle_count = 0;
};

#endif