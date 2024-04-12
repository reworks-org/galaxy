///
/// AnimationSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/components/Animated.hpp"
#include "galaxy/components/Sprite.hpp"
#include "galaxy/flags/Disabled.hpp"
#include "galaxy/scene/Scene.hpp"

#include "AnimationSystem.hpp"

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

		void AnimationSystem::update(entt::registry& registry)
		{
			const auto group = registry.group<components::Animated>(entt::get<components::Sprite>, entt::exclude<flags::Disabled>);
			for (auto&& [entity, animated, sprite] : group.each())
			{
				if (!animated.m_paused && animated.m_anim != nullptr)
				{
					animated.m_duration += (GALAXY_DT * animated.m_anim->m_speed);

					if (animated.m_duration >= animated.m_anim->current().m_duration)
					{
						animated.m_duration = 0.0;
						animated.m_anim->next();

						sprite.set_clip(animated.m_anim->current().m_bounds);
					}
				}
			}
		}
	} // namespace systems
} // namespace galaxy
