///
/// AnimationSystem.cpp
/// starlight
///
/// Created by reworks on 10/11/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/utils/Time.hpp"
#include "sl/components/SpriteComponent.hpp"
#include "sl/components/AnimationComponent.hpp"

#include "AnimationSystem.hpp"

namespace sl
{
	void AnimationSystem::event(ALLEGRO_EVENT* event, entt::DefaultRegistry& registry)
	{
	}

	void AnimationSystem::update(const double dt, entt::DefaultRegistry& registry)
	{
		auto view = registry.view<AnimationComponent, SpriteComponent>();
		for (entt::Entity entity : view)
		{
			AnimationComponent& ac = view.get<AnimationComponent>(entity);
			SpriteComponent& sc = view.get<SpriteComponent>(entity);

			if (!ac.m_isPaused)
			{
				Animation* animation = ac.m_animations.at(ac.m_activeAnimation);

				auto timepassed = (dt * animation->m_speed);
				ac.m_currentFrameTime += timepassed;

				if (ac.m_currentFrameTime >= time::milliseconds(animation->m_timePerFrame))
				{
					ac.m_currentFrameTime = 0.0;
					animation->m_currentFrame++;

					if (animation->m_currentFrame > (animation->m_totalFrames - 1))
					{
						animation->m_currentFrame = 0;

						if (!animation->m_isLooped)
						{
							ac.stop();
						}
					}

					sc.m_spriteName = animation->m_frames[animation->m_currentFrame];
				}
			}
		}
	}
}