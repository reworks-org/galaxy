///
/// Box2DManager.cpp
/// starlight
///
/// Created by reworks on 12/11/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/core/World.hpp"
#include "sl/core/ServiceLocator.hpp"
#include "sl/components/PhysicsComponent.hpp"

#include "Box2DManager.hpp"

namespace sl
{
	Box2DManager::Box2DManager(float32 gravity)
	{
		m_b2world = std::make_unique<b2World>(b2Vec2(0.0, gravity));
	}

	Box2DManager::~Box2DManager()
	{
		Locator::world->m_registry.view<PhysicsComponent>().each([this](entt::Entity entity, PhysicsComponent& pc)
		{
			for (b2Fixture* f = pc.m_body->GetFixtureList(); f; f = f->GetNext())
			{
				entt::Entity* entity = static_cast<entt::Entity*>(f->GetUserData());
				delete entity;
			}

			m_b2world->DestroyBody(pc.m_body);
		});
	}
}