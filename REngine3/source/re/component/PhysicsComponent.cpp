//
//  PhysicsComponent.cpp
//  REngine3
//
//  Created by reworks on 10/11/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <map>
#include "re/scripting/lua/lua.hpp"

#include "re/physics/Box2DManager.hpp"
#include "re/physics/Box2DSFMLBridge.hpp"
#include "re/services/ServiceLocator.hpp"

#include "PhysicsComponent.hpp"

namespace re
{
	PhysicsComponent::PhysicsComponent()
	{
        m_body = nullptr;
        m_isMovingVertically = false;
        m_isMovingHorizontally = false;
	}

	PhysicsComponent::~PhysicsComponent()
	{
		for (b2Fixture* f = m_body->GetFixtureList(); f; f = f->GetNext())
		{
			PhysicsFixtureUserData* data = static_cast<PhysicsFixtureUserData*>(f->GetUserData());
			delete data;
		}

		Locator::get<Box2DManager>()->m_world->DestroyBody(m_body);
	}

	void PhysicsComponent::init(sol::table& table)
	{
		b2BodyDef bodyDef;
		bodyDef.position.Set((float32)b2::pixelsToMeters<double>(table.get<double>("x")), (float32)b2::pixelsToMeters<double>(table.get<double>("y")));	

		// 0 = static, 1 = kinematic, 2 = dynamic
		switch (table.get<int>("bodyType"))
		{
		case 0:
			bodyDef.type = b2BodyType::b2_staticBody;
			break;

		case 1:
			bodyDef.type = b2BodyType::b2_kinematicBody;
			break;

		case 2:
			bodyDef.type = b2BodyType::b2_dynamicBody;
			break;
		}
		
		m_body = Locator::get<Box2DManager>()->m_world->CreateBody(&bodyDef);
		sol::table fixtureList = table.get<sol::table>("fixtureList");
		
		// Get key-value pairs from table
		std::map <std::string, sol::table> m_keyValuePair;
		fixtureList.for_each([&](std::pair<sol::object, sol::object> pair) {
			m_keyValuePair.insert({ pair.first.as<std::string>(), pair.second.as<sol::table>() });
		});

		RE_REVERSE_ASSERT(m_keyValuePair.empty(), "Attempted to use an empty fixture list", "PhysicsComponent::init", "PhysicsComponent.cpp", 55);

		for (auto& it : m_keyValuePair)
		{
			float32 w = it.second.get<float32>("w");
			float32 h = it.second.get<float32>("h");
			float32 angle = it.second.get<float32>("angle");
			float32 density = it.second.get<float32>("density");
			float32 friction = it.second.get<float32>("friction");
			float32 restitution = it.second.get<float32>("restitution");

			b2PolygonShape b2shape;
			b2shape.SetAsBox((float32)b2::pixelsToMeters<double>(w / 2.0), (float32)b2::pixelsToMeters<double>(h / 2.0), b2Vec2((float32)b2::pixelsToMeters<double>(w / 2.0), (float32)b2::pixelsToMeters<double>(h / 2.0)), angle);

			b2FixtureDef fixtureDef;
			fixtureDef.density = density;
			fixtureDef.friction = friction;
			fixtureDef.restitution = restitution;
			fixtureDef.shape = &b2shape;
			fixtureDef.userData = static_cast<void*>(new PhysicsFixtureUserData(it.second.get<std::string>("id")));

			m_body->CreateFixture(&fixtureDef);
		}
		
		m_body->SetFixedRotation(table.get<bool>("fixedRotation"));
	}

	bool PhysicsComponent::debugFunction(sol::table& table)
	{
		bool saveData = false;
		return saveData;
	}
}
