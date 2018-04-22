///
/// PhysicsComponent.cpp
/// starlight
///
/// Created by reworks on 10/11/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/libs/sol2/sol.hpp"
#include "sl/libs/loguru/loguru.hpp"
#include "sl/physics/Box2DHelper.hpp"
#include "sl/core/ServiceLocator.hpp"
#include "sl/physics/Box2DManager.hpp"
#include "sl/libs/Box2D/Dynamics/b2Body.h"

#include "PhysicsComponent.hpp"

namespace sl
{
	PhysicsComponent::PhysicsComponent(entt::Entity entity, const sol::table& table)
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
		
		m_body = Locator::box2dManager->m_b2world->CreateBody(&bodyDef);

		sol::table fixtureList = table.get<sol::table>("fixtureList");
		if (!fixtureList.empty())
		{
			fixtureList.for_each([&](std::pair<sol::object, sol::object> pair)
			{
				auto first = pair.first.as<std::string>();
				auto second = pair.second.as<sol::table>();

				float32 w = second.get<float32>("w");
				float32 h = second.get<float32>("h");

				b2PolygonShape b2shape;
				b2shape.SetAsBox(b2::pixelsToMeters<float32>(w / 2.0f), b2::pixelsToMeters<float32>(h / 2.0f), b2Vec2(b2::pixelsToMeters<float32>(w / 2.0f), b2::pixelsToMeters<float32>(h / 2.0f)), second.get<float32>("angle"));

				b2FixtureDef fixtureDef;
				fixtureDef.density = second.get<float32>("density");
				fixtureDef.friction = second.get<float32>("friction");
				fixtureDef.restitution = second.get<float32>("restitution");
				fixtureDef.shape = &b2shape;
				fixtureDef.userData = static_cast<void*>(new entt::Entity{ entity });
				fixtureDef.filter.categoryBits = second.get<std::uint16_t>("collisionType");

				sol::table cw = second.get<sol::table>("collidesWith");
				std::uint16_t mb = 0x000;
				
				cw.for_each([&](std::pair<sol::object, sol::object> pair)
				{
					mb = mb | pair.second.as<std::uint16_t>();
				});

				fixtureDef.filter.maskBits = mb;

				m_body->CreateFixture(&fixtureDef);
			});
		}
		else
		{
			LOG_S(WARNING) << "Fixture List is empty!";
		}
		
		m_body->SetFixedRotation(table.get<bool>("fixedRotation"));
	}

	PhysicsComponent& PhysicsComponent::operator=(const PhysicsComponent&)
	{
		return *this;
	}
}