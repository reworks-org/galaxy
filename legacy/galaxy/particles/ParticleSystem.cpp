///
/// ParticleSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <chrono>
#include <optional>

#include "galaxy/algorithm/Random.hpp"
#include "galaxy/components/ParticleGenerator.hpp"
#include "galaxy/core/World.hpp"

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

		void ParticleSystem::update(core::World& world, const double dt)
		{
			world.operate<components::ParticleGenerator>([&](const ecs::Entity entity, components::ParticleGenerator* pg) {
				pg->m_offsets.clear();
				pg->m_offsets.reserve(pg->m_amount);

				if (!pg->m_finished)
				{
					for (auto particle = pg->m_particles.begin(); particle != pg->m_particles.end();)
					{
						particle->m_life -= algorithm::random(0.01, 0.05) * dt;
						if (particle->m_life < 0.0f)
						{
							particle = pg->m_particles.erase(particle);
						}
						else
						{
							particle->move(static_cast<float>(dt));
							pg->m_offsets.emplace_back(particle->pos().x, particle->pos().y, particle->m_life);

							++particle;
						}
					}

					if (!pg->m_particles.empty())
					{
						pg->m_particles_instances[pg->m_current_instance].update_instances(pg->m_offsets);
					}
					else
					{
						pg->m_finished = true;
					}
				}
			});
		}
	} // namespace systems
} // namespace galaxy