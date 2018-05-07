///
/// ParticleSystem.cpp
/// starlight
///
/// Created by reworks on 15/01/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/utils/Random.hpp"
#include "sl/events/EventTypes.hpp"
#include "sl/libs/loguru/loguru.hpp"
#include "sl/events/ParticleEmitEvent.hpp"
#include "sl/components/RenderComponent.hpp"
#include "sl/components/ParticleComponent.hpp"
#include "sl/components/TransformComponent.hpp"

#include "ParticleSystem.hpp"

namespace sl
{
	void ParticleSystem::event(ALLEGRO_EVENT* event, entt::DefaultRegistry& registry)
	{
		switch (event->type)
		{
		case EventTypes::PARTICLE_EMIT_EVENT:
			ParticleEmitEvent* particleEvent = (ParticleEmitEvent*)event->user.data1;
			auto size = particleEvent->m_textureIDS.size();

			if (size != particleEvent->m_particleCount.size())
			{
				LOG_S(ERROR) << "Particle count and texture id are not same size std::vectors!";

				// We are forced to exit the function early, to avoid crashing the program.
				return;
			}
			else
			{
				for (std::uint32_t i = 0; i < size; ++i)
				{
					for (std::uint32_t amountOfParticles = 0; amountOfParticles < particleEvent->m_particleCount[amountOfParticles]; ++amountOfParticles)
					{
						entt::DefaultRegistry::entity_type entity = registry.create();
						
						registry.assign<TransformComponent>(entity, particleEvent->m_layer, 0.0f, Rect<float, int>(particleEvent->m_x, particleEvent->m_y, 0, 0));
						registry.assign<ParticleComponent>(entity, Random::random(particleEvent->m_upper, particleEvent->m_lower), Random::random(particleEvent->m_upper, particleEvent->m_lower), particleEvent->m_fade);
						registry.assign<RenderComponent>(entity, 1.0f, particleEvent->m_textureIDS[i]);
					}
				}
			}
			
			break;
		}
	}

	void ParticleSystem::update(const double dt, entt::DefaultRegistry& registry)
	{
		registry.view<ParticleComponent, TransformComponent, RenderComponent>()
			.each([&](entt::DefaultRegistry::entity_type entity, ParticleComponent& pc, TransformComponent& tc, RenderComponent& rc)
		{
			if (rc.m_opacity <= 0.0f)
			{
				registry.destroy(entity);
			}
			else
			{
				tc.m_rect.m_x += pc.m_direction.m_x;
				tc.m_rect.m_y += pc.m_direction.m_y;

				rc.m_opacity -= pc.m_fade;
			}
		});
	}
}