//
//  PhysicsSystem.cpp
//  REngine3
//
//  Created by reworks on 8/11/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <SFML/System/Time.hpp>

#include "re/app/World.hpp"
#include "re/physics/Box2DSFMLBridge.hpp"
#include "re/component/PhysicsComponent.hpp"
#include "re/component/TransformComponent.hpp"
#include "re/component/AnimationComponent.hpp"

#include "PhysicsSystem.hpp"

namespace re
{
	PhysicsSystem::PhysicsSystem(Box2DManager* manager, float ups, int32 vi, int32 pi)
	{
		m_manager = manager;
		m_ups = ups;
		m_velocityIterations = vi;
		m_positionIterations = pi;
		m_typeAsString = "PhysicsSystem";
	}

	PhysicsSystem::~PhysicsSystem()
	{
		m_entitys.clear();
		
		for (auto& v : m_mapCollisions)
		{
			m_manager->m_world->DestroyBody(v);
		}

		m_mapCollisions.clear();
		m_manager = nullptr;
	}

	void PhysicsSystem::addEntity(Entity* e)
	{
		if (e->m_systemIds.find("PhysicsSystem") == e->m_systemIds.end())
		{
			e->m_systemIds.emplace("PhysicsSystem", std::type_index(typeid(PhysicsSystem)));
		}

		m_entitys.emplace(e->m_name, e);
		m_entitys[e->m_name]->get<PhysicsComponent>()->m_body->SetActive(true);
	}

	void PhysicsSystem::removeEntity(const std::string& name)
	{
		m_entitys[name]->get<PhysicsComponent>()->m_body->SetActive(false);
		m_entitys.erase(name);
	}

	void PhysicsSystem::update(sf::Time dt)
	{
		m_manager->m_world->Step((float32)(1.0 / m_ups), m_velocityIterations, m_positionIterations);

		for (auto& e : m_entitys)
		{
			auto phys = e.second->get<PhysicsComponent>();
			auto transf = e.second->get<TransformComponent>();

			transf->setPosition((float)b2::metersToPixels<double>(phys->m_body->GetPosition().x), (float)b2::metersToPixels<double>(phys->m_body->GetPosition().y));
			transf->setRotation((float)b2::radToDeg<double>(phys->m_body->GetAngle()));

			if (e.second->has<AnimationComponent>() && ((phys->m_isMovingHorizontally == false) && (phys->m_isMovingVertically == true)))
			{
				e.second->get<AnimationComponent>()->pause();
			}

			if (e.second->has<AnimationComponent>() && ((phys->m_isMovingHorizontally == true) && (phys->m_isMovingVertically == true)))
			{
				e.second->get<AnimationComponent>()->pause();
			}

			if (e.second->has<AnimationComponent>() && ((phys->m_isMovingHorizontally == false) && (phys->m_isMovingVertically == false)))
			{
				e.second->get<AnimationComponent>()->pause();
			}

			if (e.second->has<AnimationComponent>() && ((phys->m_isMovingHorizontally == true) && (phys->m_isMovingVertically == false)))
			{
				e.second->get<AnimationComponent>()->play();
			}

			if (phys->m_body->GetLinearVelocity().x < 0.2f)
			{
				phys->m_isMovingHorizontally = false;
			}
		}
	}

	void PhysicsSystem::clean()
	{
		m_entitys.clear();

		for (auto& v : m_mapCollisions)
		{
			m_manager->m_world->DestroyBody(v);
		}

        m_mapCollisions.clear();
	}
}
