///
/// ParticleSystem.cpp
/// starlight
///
///  Created by reworks on 15/01/2018.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#include "loguru/loguru.hpp"
#include "sl/core/World.hpp"
#include "sl/math/Random.hpp"
#include "sl/events/EventTypes.hpp"
#include "sl/events/ParticleEmitEvent.hpp"
#include "sl/components/ParticleComponent.hpp"
#include "sl/components/TransformComponent.hpp"

#include "ParticleSystem.hpp"

namespace sl
{
	ParticleSystem::ParticleSystem()
	{
		m_world = World::get();
	}

	void ParticleSystem::event(ALLEGRO_EVENT* event)
	{
		switch (event->type)
		{
		case EventTypes::PARTICLE_EMIT_EVENT:
			auto particleEvent = static_cast<ParticleEmitEvent*>(event->user.data1);
			auto size = particleEvent->particleCount.size();

			if (size != particleEvent->textureIDS.size())
			{
				LOG_S(ERROR) << "Particle count and texture id are not same size std::vectors!";

				// We are forced to exit the function early, to avoid crashing the program.
				return;
			}
			else
			{
				for (auto i = 0; i < size; ++i)
				{
					for (auto amountOfParticles = 0; amountOfParticles < particleEvent[i]; ++amountOfParticles)
					{
						entt::Entity entity = m_world->m_registery.create();
						m_world->m_registery.assign<TransformComponent>(entity, particleEvent->layer, 0.0f, { particleEvent->xPos, particleEvent->yPos, 0, 0 });
						m_world->m_registery.assign<ParticleComponent>(entity, Random::random(particleEvent->upper, particleEvent->lower), Random::random(particleEvent->upper, particleEvent->lower), 255, particleEvent->fade, particleEvent->textureIDS[i]);
					}
				}
			}
			
			break;
		}
	}

	void ParticleSystem::update(const double dt, entt::DefaultRegistry& registery)
	{
		m_world->m_registery.view<ParticleComponent, TransformComponent>().each([this](entt::Entity e, ParticleComponent& pc, TransformComponent& tc)
		{
			if (pc.m_alpha <= 0)
			{
				m_world->m_registery.destroy(entity);
			}
			else
			{
				tc.m_rect.m_x += pc.m_direction.m_x;
				tc.m_rect.m_y += pc.m_direction.m_y;
				
				pc.m_alpha -= pc.m_fade;
			}
		});
	}
}