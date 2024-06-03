///
/// ParticleSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/components/ParticleGenerator.hpp"
#include "galaxy/components/Transform.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/flags/Disabled.hpp"
#include "galaxy/scene/Scene.hpp"
#include "galaxy/utils/Globals.hpp"

#include "ParticleSystem.hpp"

namespace galaxy
{
	namespace systems
	{
		ParticleSystem::ParticleSystem()
			: m_forces {0.0f, -9.81}
		{
		}

		ParticleSystem::~ParticleSystem()
		{
		}

		void ParticleSystem::update(scene::Scene* scene)
		{
			const auto view = scene->m_registry.view<components::ParticleGenerator>(entt::exclude<flags::Disabled>);
			for (auto&& [entity, gen] : view.each())
			{
				for (int i = 0; i < gen.m_count; i++)
				{
					gen.m_particles[i].m_life -= GALAXY_DT;

					if (gen.m_particles[i].m_life > 0)
					{
						gen.m_particles[i].m_vel += m_forces * GALAXY_DT;
						gen.m_particles[i].m_pos += gen.m_particles[i].m_vel * GALAXY_DT + 0.5f * m_forces * GALAXY_DT * GALAXY_DT;
					}
					else
					{
						gen.reset(i);
					}
				}
			}
		}
	} // namespace systems
} // namespace galaxy
