///
/// ParticleSystem.cpp
/// starlight
///
/// Created by reworks on 15/01/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/core/World.hpp"
#include "sl/utils/Random.hpp"
#include "sl/libs/loguru/loguru.hpp"
#include "sl/core/ServiceLocator.hpp"
#include "sl/components/RenderComponent.hpp"
#include "sl/components/EnabledComponent.hpp"
#include "sl/libs/entt/signal/dispatcher.hpp"
#include "sl/components/ParticleComponent.hpp"
#include "sl/components/TransformComponent.hpp"

#include "ParticleSystem.hpp"

namespace sl
{
	ParticleSystem::ParticleSystem()
	{
		// Attach collision listener.
		Locator::dispatcher->sink<EmitParticleEvent>().connect(this);
	}

	void ParticleSystem::receive(const EmitParticleEvent& pe)
	{
		for (auto& pair : pe.m_particles)
		{
			// For each particle...
			unsigned int amount = pair.second;
			for (unsigned int i = 0; i < amount; ++i)
			{
				// ...Create an entity of that particle to be rendered to screen.
				auto entity = Locator::world->m_registry.create();

				Locator::world->m_registry.assign<TransformComponent>(entity, pe.m_layer, 0.0f, Rect<float, int>(pe.m_x, pe.m_y, 0, 0));
				Locator::world->m_registry.assign<ParticleComponent>(entity, Random::random(pe.m_upper, pe.m_lower), Random::random(pe.m_upper, pe.m_lower), pe.m_fade);
				Locator::world->m_registry.assign<RenderComponent>(entity, 1.0f, pair.first);
				Locator::world->m_registry.assign<EnabledComponent>(entity);
			}
		}
	}

	void ParticleSystem::update(const double dt, entt::DefaultRegistry& registry)
	{
		// Iterate over ParticleComponent entities.
		registry.view<ParticleComponent, TransformComponent, RenderComponent, EnabledComponent>()
			.each([&](entt::DefaultRegistry::entity_type entity, ParticleComponent& pc, TransformComponent& tc, RenderComponent& rc, EnabledComponent& ec)
		{
			// If the opacity is below 0, i.e. invisible, destroy the entity.
			if (rc.m_opacity <= 0.0f)
			{
				registry.destroy(entity);
			}
			else
			{
				// Otherwise, increment the direction the particle is travelling in and fade it out a little bit more.
				tc.m_rect.m_x += pc.m_direction.m_x;
				tc.m_rect.m_y += pc.m_direction.m_y;

				rc.m_opacity -= pc.m_fade;
			}
		});
	}
}