///
///  PhysicsComponent.cpp
///  rework
///
///  Created by reworks on 10/11/2016.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#include <map>

#include "sol2/sol.hpp"
#include "loguru/loguru.hpp"
#include "imgui/imgui_impl_a5.h"
#include "Box2D/Dynamics/b2Body.h"
#include "re/physics/Box2DHelper.hpp"
#include "re/managers/Box2DManager.hpp"
#include "re/services/ServiceLocator.hpp"

#include "PhysicsComponent.hpp"

namespace re
{
	PhysicsComponent::PhysicsComponent(const sol::table& table)
	:m_body(nullptr)
	{
		b2BodyDef bodyDef;
		bodyDef.position.Set(b2::pixelsToMeters<float32>(table.get<float>("x")), b2::pixelsToMeters<float32>(table.get<float>("y")));

		/// 0 = static, 1 = kinematic, 2 = dynamic
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
		
		/// Get key-value pairs from table.
		std::map<std::string, sol::table> kvp;
		fixtureList.for_each([&](std::pair<sol::object, sol::object> pair)
		{
			kvp.insert({ pair.first.as<std::string>(), pair.second.as<sol::table>() });
		});

		if (kvp.empty())
		{
			LOG_S(FATAL) << "Attempted to use an empty fixture list.";
		}
		
		for (auto& it : kvp)
		{
			float32 w = it.second.get<float32>("w");
			float32 h = it.second.get<float32>("h");

			b2PolygonShape b2shape;
			b2shape.SetAsBox(b2::pixelsToMeters<float32>(w / 2.0f), b2::pixelsToMeters<float32>(h / 2.0f), b2Vec2(b2::pixelsToMeters<float32>(w / 2.0f), b2::pixelsToMeters<float32>(h / 2.0f)), it.second.get<float32>("angle"));

			b2FixtureDef fixtureDef;
			fixtureDef.density = it.second.get<float32>("density");
			fixtureDef.friction = it.second.get<float32>("friction");
			fixtureDef.restitution = it.second.get<float32>("restitution");
			fixtureDef.shape = &b2shape;
			fixtureDef.userData = static_cast<void*>(new std::string(it.second.get<std::string>("id")));

			m_body->CreateFixture(&fixtureDef);
		}
		
		m_body->SetFixedRotation(table.get<bool>("fixedRotation"));
	}

	PhysicsComponent::~PhysicsComponent()
	{
		/// CLEANUP IS HANDLED BY BOX2D MANAGER. THIS IS BECAUSE WE DONT KNOW WHEN DESTRUCTORS ARE CALLED, AND CLEANUP CODE MUST BE CALLED BEFORE 
		/// CLEANUP OF BOX2DMANAGER!
	}

	void PhysicsComponent::debug()
	{
		ImGui::Text("Edit the script to make changes.");

		ImGui::Spacing();

		ImGui::Text("This is done because there are issues with manually setting stuff in box2d.");
	}
}