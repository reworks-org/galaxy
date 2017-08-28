//
//  Box2DManager.cpp
//  REngine3
//
//  Created by reworks on 12/11/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "PhysicsManager.hpp"

namespace re
{
	PhysicsManager::PhysicsManager(float32 gravity)
	{
		m_gravity = b2Vec2(0.0, gravity);

		m_world = new b2World(m_gravity);
	}

	PhysicsManager::~PhysicsManager()
	{
		delete m_world;
	}

	void PhysicsManager::clean()
	{
		m_collisionFunctions.clear();
	}
}
