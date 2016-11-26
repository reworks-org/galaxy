//
//  MoveSystem.cpp
//  REngine3
//
//  Created by reworks on 07/10/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <SFML/System/Time.hpp>

#include <re/app/World.hpp>
#include <re/component/PhysicsComponent.hpp>
#include <re/component/TransformComponent.hpp>

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
			if (it.second.Has<PhysicsComponent>() && it.second.Has<TransformComponent>())
			{
				AddEntity(&it.second);
			}
		}
	}

	void MoveSystem::AddEntity(Entity* e)
	{
		e->m_systemIds.emplace("MoveSystem", std::type_index(typeid(MoveSystem)));
		m_entitys.emplace(e->m_name, e);
	}

	void MoveSystem::RemoveEntity(const std::string& name)
	{
		m_entitys.erase(name);
	}

	void MoveSystem::Move(const std::string& name, double velocity)
	{
		auto found = m_entitys.find(name);

		if (found != m_entitys.end())
		{
			auto body = found->second->Get<PhysicsComponent>()->m_body;
			auto vel = body->GetLinearVelocity();
			vel.x = velocity;
			body->SetLinearVelocity(vel);
		}
	}

	void MoveSystem::Jump(const std::string& name, double velocity)
	{
		auto found = m_entitys.find(name);

		if (found != m_entitys.end())
		{
			auto body = found->second->Get<PhysicsComponent>()->m_body;
			auto vel = body->GetLinearVelocity();
			vel.y = velocity;
			body->SetLinearVelocity(vel);
		}
	}

	void MoveSystem::Clean()
	{
		m_entitys.clear();
	}
}