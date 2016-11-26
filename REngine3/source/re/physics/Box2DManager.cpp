//
//  Box2DManager.cpp
//  REngine3
//
//  Created by reworks on 12/11/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "Box2DManager.hpp"

namespace re
{
	Box2DManager::Box2DManager(double gravity)
		:m_world(b2Vec2(0.0, gravity))
	{
		m_gravity = b2Vec2(0.0, gravity);
	}
}