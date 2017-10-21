//
//  PhysicsComponent.cpp
//  rework
//
//  Created by reworks on 10/11/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include <map>

#include "imgui/imgui.h"
#include "re/utils/Log.hpp"
#include "re/physics/Box2DHelper.hpp"
#include "re/managers/Box2DManager.hpp"
#include "re/services/ServiceLocator.hpp"

#include "PhysicsComponent.hpp"

namespace re
{
	PhysicsComponent::PhysicsComponent(sol::table& table)
	:m_body(nullptr)
	{
		b2BodyDef bodyDef;
		bodyDef.position.Set(b2::pixelsToMeters<float32>(table.get<float32>("x")), b2::pixelsToMeters<float32>(table.get<float32>("y")));

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
		
		m_body = Locator::get<Box2DManager>()->world()->CreateBody(&bodyDef);
		sol::table fixtureList = table.get<sol::table>("fixtureList");
		
		// Get key-value pairs from table
		std::map<std::string, sol::table> kvp;
		fixtureList.for_each([&](std::pair<sol::object, sol::object> pair) {
			kvp.insert({ pair.first.as<std::string>(), pair.second.as<sol::table>() });
		});

		if (kvp.empty())
		{
			BOOST_LOG_TRIVIAL(warning) << "Attempted to use an empty fixture list." << std::endl;
		}
		
		for (auto& it : kvp)
		{
			float32 w = it.second.get<float32>("w");
			float32 h = it.second.get<float32>("h");
			float32 angle = it.second.get<float32>("angle");
			float32 density = it.second.get<float32>("density");
			float32 friction = it.second.get<float32>("friction");
			float32 restitution = it.second.get<float32>("restitution");

			b2PolygonShape b2shape;
			b2shape.SetAsBox(b2::pixelsToMeters<float32>(w / 2.0f), b2::pixelsToMeters<float32>(h / 2.0f), b2Vec2(b2::pixelsToMeters<float32>(w / 2.0f), b2::pixelsToMeters<float32>(h / 2.0f)), angle);

			b2FixtureDef fixtureDef;
			fixtureDef.density = density;
			fixtureDef.friction = friction;
			fixtureDef.restitution = restitution;
			fixtureDef.shape = &b2shape;
			fixtureDef.userData = static_cast<void*>(new std::string(it.second.get<std::string>("id")));

			m_body->CreateFixture(&fixtureDef);
		}
		
		m_body->SetFixedRotation(table.get<bool>("fixedRotation"));
	}

	PhysicsComponent::~PhysicsComponent()
	{
		if (m_body)
		{
			if (m_body->GetFixtureList())
			{
				for (b2Fixture* f = m_body->GetFixtureList(); f; f = f->GetNext())
				{
					if (f)
					{
						if (f->GetUserData())
						{
							std::string* data = static_cast<std::string*>(f->GetUserData());
							delete data;
						}
					}
				}
			}
		}
		
		Locator::get<Box2DManager>()->world()->DestroyBody(m_body);
	}

	void PhysicsComponent::debug()
	{
		ImGui::Text("Edit the script to make changes.");

		ImGui::Spacing();

		ImGui::Text("This is done because there are issues with manually setting stuff in box2d.");
	}
}