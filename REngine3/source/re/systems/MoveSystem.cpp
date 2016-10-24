//
//  MoveSystem.cpp
//  REngine3
//
//  Created by reworks on 07/10/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <SFML/System/Time.hpp>

#include "re/app/World.hpp"
#include "re/component/PositionComponent.hpp"

#include "MoveSystem.hpp"

namespace re
{
	MoveSystem::~MoveSystem()
	{
		m_entitys.clear();
	}

	void MoveSystem::AutoSubmit(World* world)
	{
		for (auto& it : world->GetAlive())
		{
			if (it.second->Has<PositionComponent>())
			{
				AddEntity(it.second);
			}
		}
	}

	void MoveSystem::AddEntity(std::shared_ptr<Entity> e)
	{
		e->m_systemIds.emplace("MoveSystem", std::type_index(typeid(MoveSystem)));
		m_entitys.emplace(e->m_name, e);
	}

	void MoveSystem::RemoveEntity(const std::string& name)
	{
		m_entitys.erase(name);
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