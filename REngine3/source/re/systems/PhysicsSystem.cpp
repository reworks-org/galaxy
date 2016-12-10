//
//  PhysicsSystem.cpp
//  REngine3
//
//  Created by reworks on 8/11/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <SFML/System/Time.hpp>

#include "re/app/World.hpp"
#include "re/mapping/TMXMap.hpp"
#include "re/physics/Box2DSFMLBridge.hpp"
#include "re/component/PhysicsComponent.hpp"
#include "re/component/TransformComponent.hpp"
#include "re/component/AnimationComponent.hpp"

#include "PhysicsSystem.hpp"

namespace re
{
	PhysicsSystem::PhysicsSystem(Box2DManager* manager, double ups, double vi, double pi)
	{
		m_manager = manager;
		m_ups = ups;
		m_velocityIterations = vi;
		m_positionIterations = pi;
	}

	PhysicsSystem::~PhysicsSystem()
	{
		m_entitys.clear();
		
		for (auto& v : m_mapCollisions)
		{
			m_manager->m_world.DestroyBody(v);
		}

		m_mapCollisions.clear();
		m_manager = nullptr;
	}

	void PhysicsSystem::AutoSubmit(World* world)
	{
		for (auto& it : world->GetAlive())
		{
			if (it.second.Has<PhysicsComponent>() && it.second.Has<TransformComponent>())
			{
				AddEntity(&it.second);
			}
		}
	}

	void PhysicsSystem::SubmitTiles(TMXMap* map)
	{
		for (auto& v : map->GetCollisions())
		{
			b2BodyDef bodyDef;
			bodyDef.position.Set((double)v.left, (double)v.top);

			bodyDef.type = b2BodyType::b2_staticBody;

			b2PolygonShape b2shape;
			b2shape.SetAsBox((double)v.width / 2.0, (double)v.height / 2.0);

			b2FixtureDef fixtureDef;
			fixtureDef.density = 1;
			fixtureDef.friction = 50;
			fixtureDef.restitution = 0;
			fixtureDef.shape = &b2shape;

			b2Body* body = m_manager->m_world.CreateBody(&bodyDef);
			body->CreateFixture(&fixtureDef);
			m_mapCollisions.push_back(body);
		}
	}

	void PhysicsSystem::AddEntity(Entity* e)
	{
		e->m_systemIds.emplace("PhysicsSystem", std::type_index(typeid(PhysicsSystem)));
		// we need to set the body's user data to the entity.
		e->Get<PhysicsComponent>()->m_body->SetUserData(static_cast<void*>(e));
		m_entitys.emplace(e->m_name, e);
	}

	void PhysicsSystem::RemoveEntity(const std::string& name)
	{
		m_entitys.erase(name);
	}

	void PhysicsSystem::Update(sf::Time dt)
	{
		m_manager->m_world.Step(1.0 / m_ups, m_velocityIterations, m_positionIterations);

		for (auto& e : m_entitys)
		{
			auto phys = e.second->Get<PhysicsComponent>();
			auto transf = e.second->Get<TransformComponent>();

			transf->setPosition(b2::MetersToPixels<double>(phys->m_body->GetPosition().x), b2::MetersToPixels<double>(phys->m_body->GetPosition().y));
			transf->setRotation(b2::RadToDeg<double>(phys->m_body->GetAngle()));

			if (e.second->Has<AnimationComponent>() && (!phys->m_isMovingHoritontally))
			{
				e.second->Get<AnimationComponent>()->Pause();
			}

			if (phys->m_body->GetLinearVelocity().x < 0.2f)
			{
				phys->m_isMovingHoritontally = false;
			}
		}
	}

	void PhysicsSystem::Clean()
	{
		m_entitys.clear();

		for (auto& v : m_mapCollisions)
		{
			m_manager->m_world.DestroyBody(v);
		}

		m_mapCollisions.clear();

		// We do not reset the manager pointer because we want to reuse it.
	}
}