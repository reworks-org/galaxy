//
//  CollisionEvent.cpp
//  rework
//
//  Created by reworks on 2/11/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include "re/managers/Box2DManager.hpp"
#include "re/services/ServiceLocator.hpp"


#include "CollisionEvent.hpp"

namespace re
{
	CollisionEvent::CollisionEvent(entityx::Entity& left, entityx::Entity& right)
		:m_left(left), m_right(right)
	{
	}

	void CollisionEvent::handleCollision()
	{

	}
}