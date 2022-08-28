///
/// AnimationSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/components/Animated.hpp"
#include "galaxy/components/Sprite.hpp"
#include "galaxy/state/Layer.hpp"
#include "galaxy/utils/Globals.hpp"

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

		void AnimationSystem::update(state::Layer* layer)
		{
			const auto prim_view = layer->world().m_registry.view<components::Animated>();
			for (auto&& [entity, animated] : prim_view.each())
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
									auto sprite = layer->world().m_registry.try_get<components::Sprite>(entity);
									if (sprite != nullptr)
									{
										sprite->update(next->m_texture_id);
									}
								}
							}
						}
					}
				}
			}
		}
	} // namespace systems
} // namespace galaxy