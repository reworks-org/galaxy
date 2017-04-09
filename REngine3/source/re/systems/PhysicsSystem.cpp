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
	PhysicsSystem::PhysicsSystem(Box2DManager* manager, float ups, float vi, float pi)
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

	void PhysicsSystem::submit(World* world)
	{
		for (auto& it : world->getAliveEntitys())
		{
			if (it.second.has<PhysicsComponent>() && it.second.has<TransformComponent>())
			{
				addEntity(&it.second);
			}
		}
	}

	void PhysicsSystem::addMapCollisions(TMXMap* map)
	{
		for (auto& v : map->getCollisions())
		{
			b2BodyDef bodyDef;
			bodyDef.position.Set((float)b2::pixelsToMeters<double>(v.left), (float)b2::pixelsToMeters<double>(v.top));

			bodyDef.type = b2BodyType::b2_staticBody;

			b2PolygonShape b2shape;
			b2shape.SetAsBox(b2::pixelsToMeters<double>(v.width / 2.0), b2::pixelsToMeters<double>(v.height / 2.0), b2Vec2(b2::pixelsToMeters<double>(v.width / 2.0), b2::pixelsToMeters<double>(v.height / 2.0)), 0);

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

	void PhysicsSystem::addEntity(Entity* e)
	{
		e->m_systemIds.emplace("PhysicsSystem", std::type_index(typeid(PhysicsSystem)));
		// we need to set the body's user data to the entity.
		e->get<PhysicsComponent>()->m_body->SetUserData(static_cast<void*>(e));
		m_entitys.emplace(e->m_name, e);
	}

	void PhysicsSystem::removeEntity(const std::string& name)
	{
		m_entitys.erase(name);
	}

	void PhysicsSystem::update(sf::Time dt)
	{
		m_manager->m_world.Step(1.0 / m_ups, m_velocityIterations, m_positionIterations);

		for (auto& e : m_entitys)
		{
			auto phys = e.second->get<PhysicsComponent>();
			auto transf = e.second->get<TransformComponent>();

			transf->setPosition(b2::metersToPixels<double>(phys->m_body->GetPosition().x), b2::metersToPixels<double>(phys->m_body->GetPosition().y));
			transf->setRotation(b2::radToDeg<double>(phys->m_body->GetAngle()));

			if (e.second->has<AnimationComponent>() && (!phys->m_isMovingHoritontally))
			{
				e.second->get<AnimationComponent>()->pause();
			}

			if (phys->m_body->GetLinearVelocity().x < 0.2f)
			{
				phys->m_isMovingHoritontally = false;
			}
		}
	}

	void PhysicsSystem::clean()
	{
		m_entitys.clear();

		for (auto& v : m_mapCollisions)
		{
			m_manager->m_world.DestroyBody(v);
		}

        m_mapCollisions.clear();
	}
}
