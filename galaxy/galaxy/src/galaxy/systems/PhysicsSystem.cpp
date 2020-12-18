///
/// PhysicsSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <redshift/World.hpp>

#include "galaxy/components/All.hpp"
#include "galaxy/core/ServiceLocator.hpp"

#include "PhysicsSystem.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	PhysicsSystem::PhysicsSystem()
	{
		m_game_world = SL_HANDLE.world();
	}

	PhysicsSystem::~PhysicsSystem()
	{
	}

	void PhysicsSystem::events()
	{
	}

	void PhysicsSystem::update(const double dt)
	{
		m_game_world->phys_world().step(dt);

		m_game_world->operate<SpriteComponent, PhysicsComponent, EnabledComponent>([&](const sr::Entity entity, SpriteComponent* sprite, PhysicsComponent* phys, EnabledComponent* ef) {
			auto pos = phys->m_body->get_pos();
			sprite->m_sprite.set_pos(pos.x, pos.y);
		});
	}
} // namespace galaxy