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
#include "sl/graphics/RenderType.hpp"
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
						entt::Entity entity = registry.create();
						
						registry.assign<TransformComponent>(entity, particleEvent->m_layer, 0.0f, Rect<float, int>(particleEvent->m_x, particleEvent->m_y, 0, 0));
						registry.assign<ParticleComponent>(entity, Random::random(particleEvent->m_upper, particleEvent->m_lower), Random::random(particleEvent->m_upper, particleEvent->m_lower), 255.0f, particleEvent->m_fade, particleEvent->m_textureIDS[i]);
						RenderComponent& rc = registry.assign<RenderComponent>(entity);
						rc.m_renderTypes.resize(1);
						rc.m_renderTypes[0] = RenderTypes::PARTICLE;
					}
				}
			}
			
			break;
		}
	}

	void ParticleSystem::update(const double dt, entt::DefaultRegistry& registry)
	{
		auto view = registry.view<ParticleComponent, TransformComponent>();
		for (entt::Entity entity : view)
		{
			ParticleComponent& pc = view.get<ParticleComponent>(entity);
			TransformComponent& tc = view.get<TransformComponent>(entity);

			if (pc.m_alpha <= 0)
			{
				registry.destroy(entity);
			}
			else
			{
				tc.m_rect.m_x += pc.m_direction.m_x;
				tc.m_rect.m_y += pc.m_direction.m_y;

				pc.m_alpha -= pc.m_fade;
			}
		}
	}
}