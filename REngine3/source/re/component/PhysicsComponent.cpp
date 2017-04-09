//
//  PhysicsComponent.cpp
//  REngine3
//
//  Created by reworks on 10/11/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "re/physics/Box2DManager.hpp"
#include "re/physics/Box2DSFMLBridge.hpp"
#include "re/services/ServiceLocator.hpp"

#include "PhysicsComponent.hpp"

namespace re
{
	PhysicsComponent::PhysicsComponent()
	{
	}

	PhysicsComponent::~PhysicsComponent()
	{
		Locator::get<Box2DManager>()->m_world.DestroyBody(m_body);
	}

	void PhysicsComponent::init(sol::table& table)
	{
		b2BodyDef bodyDef;
		bodyDef.position.Set(b2::pixelsToMeters<double>(table.get<double>("x")), b2::pixelsToMeters<double>(table.get<double>("y")));

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

		b2PolygonShape b2shape;
		b2shape.SetAsBox(b2::pixelsToMeters<double>(table.get<double>("w") / 2.0), b2::pixelsToMeters<double>(table.get<double>("h") / 2.0), b2Vec2(b2::pixelsToMeters<double>(table.get<double>("w") / 2.0), b2::pixelsToMeters<double>(table.get<double>("h") / 2.0)), 0);

		b2FixtureDef fixtureDef;
		fixtureDef.density = table.get<double>("density");
		fixtureDef.friction = table.get<double>("friction");
		fixtureDef.restitution = table.get<double>("restitution");
		fixtureDef.shape = &b2shape;

		m_body = Locator::get<Box2DManager>()->m_world.CreateBody(&bodyDef);
		m_body->CreateFixture(&fixtureDef);

		m_body->SetFixedRotation(table.get<bool>("fixedRotation"));
	}
}
