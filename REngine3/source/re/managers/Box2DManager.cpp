//
//  Box2DManager.cpp
//  REngine3
//
//  Created by reworks on 12/11/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include "Box2DManager.hpp"

namespace re
{
	Box2DManager::Box2DManager(float32 gravity)
	:m_gravity(b2Vec2(0.0, gravity))
	{
		m_world = new b2World(m_gravity);
	}

	Box2DManager::~Box2DManager()
	{
		m_collisionFunctions.clear();

		delete m_world;
	}

	void Box2DManager::clean()
	{
		m_collisionFunctions.clear();
	}
}
