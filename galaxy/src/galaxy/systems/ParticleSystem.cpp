///
/// ParticleSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/GalaxyConfig.hpp"
#include "galaxy/components/ParticleEffect.hpp"
#include "galaxy/components/Transform2D.hpp"
#include "galaxy/math/Random.hpp"

#include "ParticleSystem.hpp"

namespace galaxy
{
	namespace systems
	{
		ParticleSystem::ParticleSystem() noexcept
		{
		}

		ParticleSystem::~ParticleSystem() noexcept
		{
		}

		void ParticleSystem::update(core::Scene2D* scene)
		{
			// clang-format off
			scene->m_world.operate<components::ParticleEffect>([&](const ecs::Entity entity, components::ParticleEffect* particle_effect) {
				auto& particles = particle_effect->get_particles();

				for (auto particle = particles.begin(); particle != particles.end();)
				{
					particle->m_life -= static_cast<float>(math::random(0.1, 0.3) * GALAXY_DT);

					if (particle->m_life < 0.0f)
					{
						particle = particles.erase(particle);
					}
					else
					{
						particle->move();
					}

					if (particle != particles.end())
					{
						++particle;
					}
				}

				if (!particles.empty())
				{
					particle_effect->buffer();
				}
				else
				{
					scene->m_world.disable(entity);
				}
			});
			// clang-format on
		}
	} // namespace systems
} // namespace galaxy