//
//  MoveSystem.cpp
//  REngine3
//
//  Created by reworks on 07/10/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <SFML/System/Time.hpp>

#include "re/entity/EntityManager.hpp"
#include "re/services/ServiceLocator.hpp"
#include "re/component/PositionComponent.hpp"

#include "MoveSystem.hpp"

namespace re
{
	MoveSystem::~MoveSystem()
	{
		m_entitys.clear();
	}

	void MoveSystem::AutoSubmit()
	{
		for (auto& it : Locator::Get<EntityManager>()->GetMap())
		{
			if (it.second->Has<PositionComponent>())
			{
				AddEntity(it.second);
			}
		}
	}

	void MoveSystem::AddEntity(Entity* e)
	{
		e->m_systemIds.push_back(typeid(MoveSystem));
		m_entitys.emplace(e->m_id, e);
	}

	void MoveSystem::RemoveEntity(sf::Uint64 e)
	{
		auto it = std::find(m_entitys[e]->m_systemIds.begin(), m_entitys[e]->m_systemIds.end(), typeid(MoveSystem));
		m_entitys[e]->m_systemIds.erase(it);
		m_entitys.erase(e);
	}

	void MoveSystem::Update(sf::Time dt)
	{
		for (auto& e : m_entitys)
		{
			for (auto& it : e.second->m_components)
			{
				it.second->Update(dt, e.second->Get<PositionComponent>()->m_xpos, e.second->Get<PositionComponent>()->m_ypos);
			}
		}
	}

	void MoveSystem::Clean()
	{
		m_entitys.clear();
	}
}