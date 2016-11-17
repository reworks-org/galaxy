//
//  CollisionSystem.cpp
//  REngine3
//
//  Created by reworks on 8/11/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <SFML/System/Time.hpp>

#include "re/app/World.hpp"
#include "re/mapping/TMXMap.hpp"
#include "re/component/CollisionComponent.hpp"
#include "re/component/TransformComponent.hpp"
#include "re/physics/Box2DSFMLBridge.hpp"

#include "CollisionSystem.hpp"

namespace re
{
	CollisionSystem::CollisionSystem(double ups, double vi, double pi)
	{
		m_ups = ups;
		m_velocityIterations = vi;
		m_positionIterations = pi;
	}

	CollisionSystem::~CollisionSystem()
	{
		m_entitys.clear();
		
		for (auto& v : m_mapCollisions)
		{
			m_manager->m_world.DestroyBody(v);
		}

		m_mapCollisions.clear();
		m_manager = nullptr;
	}

	void CollisionSystem::AutoSubmit(World* world)
	{
		for (auto& it : world->GetAlive())
		{
			if (it.second.Has<CollisionComponent>())
			{
				AddEntity(&it.second);
			}
		}
	}

	void CollisionSystem::ProvideManager(Box2DManager* manager)
	{
		m_manager = manager;
	}

	void CollisionSystem::SubmitTiles(TMXMap* map)
	{
		for (auto& v : map->GetCollisions())
		{
			b2BodyDef bodyDef;
			bodyDef.position.Set((double)v.left, (double)v.top);

			bodyDef.type = b2BodyType::b2_staticBody;

			b2PolygonShape b2shape;
			b2shape.SetAsBox((double)v.width / 2.0, (double)v.height / 2.0);

			b2FixtureDef fixtureDef;
			fixtureDef.density = 0;
			fixtureDef.friction = 0;
			fixtureDef.restitution = 0;
			fixtureDef.shape = &b2shape;

			b2Body* body = m_manager->m_world.CreateBody(&bodyDef);
			body->CreateFixture(&fixtureDef);
			m_mapCollisions.push_back(body);
		}
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
		m_manager->m_world.Step(1.0 / m_ups, m_velocityIterations, m_positionIterations);

		for (auto& e : m_entitys)
		{
			auto col = e.second->Get<CollisionComponent>()->m_body->GetPosition();
			e.second->Get<TransformComponent>()->setPosition(b2::MetersToPixels<double>(col.x), b2::MetersToPixels<double>(col.y));
		}
	}

	void CollisionSystem::Clean()
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