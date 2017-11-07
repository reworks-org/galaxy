//
//  Box2DManager.cpp
//  rework
//
//  Created by reworks on 12/11/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include "Box2DManager.hpp"
#include "re/components/PhysicsComponent.hpp"
#include "re/services/ServiceLocator.hpp"
#include "re/core/World.hpp"

namespace re
{
	Box2DManager::Box2DManager(float32 gravity)
	{
		m_world = new b2World(b2Vec2(0.0, gravity));
	}

	Box2DManager::~Box2DManager()
	{
		m_collisionFunctions.clear();

		Locator::get<World>()->m_entityManager.each<PhysicsComponent>([this](entityx::Entity& e, PhysicsComponent& pc)
		{
			if (pc.m_body)
			{
				if (pc.m_body->GetFixtureList())
				{
					for (b2Fixture* f = pc.m_body->GetFixtureList(); f; f = f->GetNext())
					{
						if (f)
						{
							if (f->GetUserData())
							{
								std::string* data = static_cast<std::string*>(f->GetUserData());

								if (data)
								{
									delete data;
								}
							}
						}
					}
				}
			}

			m_world->DestroyBody(pc.m_body);
		});

		delete m_world;
	}

	void Box2DManager::clean()
	{
		m_collisionFunctions.clear();
	}

	b2World* Box2DManager::world()
	{
		return m_world;
	}

	CollisionFunctionMap& Box2DManager::getCollisionFunctions()
	{
		return m_collisionFunctions;
	}
}