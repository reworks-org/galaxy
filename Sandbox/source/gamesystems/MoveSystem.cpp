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
	MoveSystem::MoveSystem()
	{
		m_typeAsString = "MoveSystem";
	}

	MoveSystem::~MoveSystem()
	{
		m_entitys.clear();
	}

	void MoveSystem::addEntity(Entity* e)
	{
		if (e->m_systemIds.find("MoveSystem") == e->m_systemIds.end())
		{
			e->m_systemIds.emplace("MoveSystem", std::type_index(typeid(MoveSystem)));
		}
		
		m_entitys.emplace(e->m_name, e);
	}

	void MoveSystem::removeEntity(const std::string& name)
	{
		m_entitys.erase(name);
	}

	void MoveSystem::move(const std::string& name, double velocity)
	{
        auto found = m_entitys.find(name);
        
        if (found != m_entitys.end())
        {
            auto phys = found->second->get<PhysicsComponent>();
            auto vel = phys->m_body->GetLinearVelocity();
            

			if (phys->m_body->GetLinearVelocity().x < 0.2f)
			{
				phys->m_isMovingHorizontally = false;
			}

            if (!phys->m_isMovingVertically)
            {
                phys->m_isMovingHorizontally = true;
                vel.x = velocity;
                phys->m_body->SetLinearVelocity(vel);
            }
        }
	}

	void MoveSystem::jump(const std::string& name, double velocity)
	{
		velocity *= -1;
		auto found = m_entitys.find(name);

		if (found != m_entitys.end())
		{
			auto phys = found->second->get<PhysicsComponent>();
			auto vel = phys->m_body->GetLinearVelocity();

			if (!phys->m_isMovingVertically)
			{
				phys->m_isMovingVertically = true;
				vel.y = velocity;
				phys->m_body->SetLinearVelocity(vel);
			}
		}
	}

	void MoveSystem::clean()
	{
		m_entitys.clear();
	}
}
