///
/// AnimationSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/GalaxyConfig.hpp"
#include "galaxy/components/Animated.hpp"
#include "galaxy/components/BatchSprite.hpp"

#include "AnimationSystem.hpp"

namespace galaxy
{
	namespace systems
	{
		AnimationSystem::AnimationSystem() noexcept
		{
		}

		AnimationSystem::~AnimationSystem() noexcept
		{
		}

		void AnimationSystem::update(core::Scene2D* scene)
		{
			scene->m_world.operate<components::Animated, components::BatchSprite>(
				std::execution::par,
				[&](const ecs::Entity entity, components::Animated* animated, components::BatchSprite* sprite)
				{
					GALAXY_UNUSED(entity);

					if (!animated->m_paused)
					{
						animated->m_time_spent_on_frame += (GALAXY_DT * animated->m_active_anim->get_speed());
						if (animated->m_time_spent_on_frame >= animated->m_active_anim->get_current_frame()->m_time_per_frame)
						{
							animated->m_time_spent_on_frame = 0;

							const auto* new_frame = animated->m_active_anim->next_frame();
							sprite->update_region(new_frame->get_region_id());
						}
					}
				});
		}
	} // namespace systems
} // namespace galaxy