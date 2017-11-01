//
//  CollisionSystem.cpp
//  rework
//
//  Created by reworks on 28/08/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include "entityx/entityx.h"
#include "re/services/ServiceLocator.hpp"
#include "re/components/CollisionComponent.hpp"
#include "re/components/TransformComponent.hpp"

#include "CollisionSystem.hpp"

namespace re
{
	CollisionSystem::CollisionSystem()
	:m_quadtree(nullptr)
	{
	}

	CollisionSystem::~CollisionSystem()
	{
		if (m_quadtree != nullptr)
		{
			m_quadtree->clear();
			delete m_quadtree;
		}
	}

	void CollisionSystem::setUpQuadTree(int level, Rect<float, int>& bounds, int maxLevels, int maxObjects)
	{
		m_quadtree = new QuadTree(level, bounds, maxLevels, maxObjects);
	}

	void CollisionSystem::update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt)
	{
		m_quadtree->clear();
		
		es.each<CollisionComponent>([this](entityx::Entity& e, CollisionComponent& cc)
		{
			m_quadtree->insert(e);
		});

		es.each<CollisionComponent, TransformComponent>([this](entityx::Entity& e, CollisionComponent& cc, TransformComponent& tc)
		{
			m_possibleCollisions.clear();
			m_quadtree->retrieve(m_possibleCollisions, e);
			
			for (auto& elem : m_possibleCollisions)
			{
				if (elem.component<TransformComponent>()->m_rect.overlaps(tc.m_rect))
				{
					/*
					auto map = Locator::get<PhysicsManager>()->m_collisionFunctions;
					auto tree = map.find(std::make_pair(a, b));

					if (tree != map.end())
					{
						tree->second(a, b);
					}
					*/
				}
			}
		});
	}

	void CollisionSystem::clean()
	{
		m_quadtree->clear();
	}
}