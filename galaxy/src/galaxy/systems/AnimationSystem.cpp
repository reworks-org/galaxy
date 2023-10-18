///
/// AnimationSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/components/Animated.hpp"
#include "galaxy/components/Sprite.hpp"
#include "galaxy/flags/Disabled.hpp"
#include "galaxy/resource/TextureAtlas.hpp"
#include "galaxy/scene/Scene.hpp"
#include "galaxy/utils/Globals.hpp"

#include "AnimationSystem.hpp"

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_PUSH
GALAXY_DISABLE_WARNING(26462)
#endif

namespace galaxy
{
	namespace systems
	{
		AnimationSystem::AnimationSystem()
		{
		}

		AnimationSystem::~AnimationSystem()
		{
		}

		void AnimationSystem::update(scene::Scene* scene)
		{
			const auto group = scene->m_world.m_registry.group<components::Animated>(entt::get<components::Sprite>, entt::exclude<flags::Disabled>);
			for (auto&& [entity, animated, sprite] : group.each())
			{
				if (!animated.is_paused())
				{
					const auto active_anim = animated.active();
					if (active_anim != nullptr)
					{
						animated.m_time_spent_on_frame += (GALAXY_DT * active_anim->m_speed);

						const auto current = active_anim->current_frame();
						if (current != nullptr)
						{
							if (animated.m_time_spent_on_frame >= current->m_time_per_frame)
							{
								animated.m_time_spent_on_frame = 0;

								const auto next = active_anim->next_frame();
								if (next != nullptr)
								{
									sprite.update(next->m_texture_id);
								}
							}
						}
					}
				}
			}
		}
	} // namespace systems
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_POP
#endif
