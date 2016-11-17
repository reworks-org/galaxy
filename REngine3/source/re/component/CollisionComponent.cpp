//
//  CollisionComponent.cpp
//  REngine3
//
//  Created by reworks on 10/11/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "re/physics/Box2DManager.hpp"
#include "re/physics/Box2DSFMLBridge.hpp"
#include "re/services/ServiceLocator.hpp"

#include "CollisionComponent.hpp"

namespace re
{
	CollisionComponent::CollisionComponent()
	{
	}

	CollisionComponent::~CollisionComponent()
	{
		Locator::Get<Box2DManager>()->m_world.DestroyBody(m_body);
	}

	void CollisionComponent::Init(sol::table& table)
	{
		b2BodyDef bodyDef;
		bodyDef.position.Set(b2::PixelsToMeters<double>(table.get<double>("startX")), b2::PixelsToMeters<double>(table.get<double>("startY")));

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
		b2shape.SetAsBox(b2::PixelsToMeters<double>(table.get<double>("w") / 2.0), b2::PixelsToMeters<double>(table.get<double>("h") / 2.0));

		b2FixtureDef fixtureDef;
		fixtureDef.density = table.get<double>("density");
		fixtureDef.friction = table.get<double>("friction");
		fixtureDef.restitution = table.get<double>("restitution");
		fixtureDef.shape = &b2shape;

		m_body = Locator::Get<Box2DManager>()->m_world.CreateBody(&bodyDef);
		m_body->CreateFixture(&fixtureDef);

		m_body->SetFixedRotation(table.get<bool>("fixedRotation"));

		//sf::Shape* shape = new sf::RectangleShape(sf::Vector2f(size_x, size_y));
		//shape->setOrigin(size_x / 2.0, size_y / 2.0);
		//shape->setPosition(sf::Vector2f(pos_x, pos_y));
	}
}