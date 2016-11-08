//
//  CollisionSystem.cpp
//  REngine3
//
//  Created by reworks on 8/11/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "re/app/World.hpp"
#include "CollisionSystem.hpp"
#include "re/component/PositionComponent.hpp"
#include "re/utility/Log.hpp"

namespace re
{
	CollisionSystem::~CollisionSystem()
	{
		m_entitys.clear();
		m_collisions.clear();
		m_tilesAroundPlayer.clear();
	}

	void CollisionSystem::AutoSubmit(World* world)
	{
		for (auto& it : world->GetAlive())
		{
			if (it.second.Has<PositionComponent>())
			{
				AddEntity(&it.second);
			}
		}
	}

	void CollisionSystem::SubmitTiles(std::vector<sf::IntRect>& ct)
	{
		for (auto& v : ct)
		{
			m_collisions.push_back(v);
		}
	}

	void CollisionSystem::AddCollision(sf::IntRect& rect)
	{
		m_collisions.push_back(rect);
	}

	void CollisionSystem::AddEntity(Entity* e)
	{
		e->m_systemIds.emplace("CollisionSystem", std::type_index(typeid(CollisionSystem)));
		m_entitys.emplace(e->m_name, e);
	}

	void CollisionSystem::RemoveEntity(const std::string& name)
	{
		m_entitys.erase(name);
	}

	void CollisionSystem::Update(sf::Time dt)
	{
		for (auto& v : m_collisions)
		{
			for (auto& e : m_entitys)
			{
				auto p = e.second->Get<PositionComponent>();
				sf::IntRect pc(p->m_xpos, p->m_ypos, p->m_width, p->m_height);
				if (pc.intersects(v) == true)
				{
					p->m_isColliding = true;
					RE_LOG(LogLevel::WARNING, "COLLIDING!");
				}
				else
				{
					p->m_isColliding = false;
				}
			}
		}
	}

	void CollisionSystem::Clean()
	{
		m_entitys.clear();
		m_collisions.clear();
		m_tilesAroundPlayer.clear();
	}
}