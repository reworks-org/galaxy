//
//  CollisionSystem.cpp
//  REngine3
//
//  Created by reworks on 28/08/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include "re/physics/PhysicsManager.hpp"
#include "re/services/ServiceLocator.hpp"
#include "re/component/CollisionComponent.hpp"

#include "CollisionSystem.hpp"

namespace re
{
	CollisionSystem::CollisionSystem()
	{
		m_typeAsString = "CollisionSystem";
	}

	CollisionSystem::~CollisionSystem()
	{
		m_quadtree->clear();
		m_entitys.clear();

		delete m_quadtree;
	}

	void CollisionSystem::addEntity(Entity* e)
	{
		if (e->m_systemIds.find("CollisionSystem") == e->m_systemIds.end())
		{
			e->m_systemIds.emplace("CollisionSystem", std::type_index(typeid(CollisionSystem)));
		}

		m_entitys.emplace(e->m_name, e);
	}

	void CollisionSystem::removeEntity(const std::string& name)
	{
		m_entitys.erase(name);
	}

	void CollisionSystem::setUpQuadTree(int level, sf::Rect<int>& bounds, int maxLevels, int maxObjects)
	{
		m_quadtree = new QuadTree(level, bounds, maxLevels, maxObjects);
	}

	void CollisionSystem::update()
	{
		m_quadtree->clear();
		for (auto& it : m_entitys)
		{
			m_quadtree->insert(it.second);
		}

		std::vector<Entity*> m_possibleCollisions;
		for (auto& it : m_entitys)
		{
			m_possibleCollisions.clear();
			m_quadtree->retrieve(m_possibleCollisions, it.second);

			for (auto& elem : m_possibleCollisions)
			{
				if (it.second->get<CollisionComponent>()->m_rect.intersects(elem->get<CollisionComponent>()->m_rect))
				{
					std::string a = it.second->m_name;
					std::string b = elem->m_name;

					auto map = Locator::get<PhysicsManager>()->m_collisionFunctions;
					auto tree = map.find(std::make_pair(a, b));

					if (tree != map.end())
					{
						tree->second(a, b);
					}
				}
			}
		}
	}

	void CollisionSystem::clean()
	{
		m_quadtree->clear();
		m_entitys.clear();
	}
}