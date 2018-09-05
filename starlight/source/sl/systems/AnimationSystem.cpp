///
/// AnimationSystem.cpp
/// starlight
///
/// Created by reworks on 10/11/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/utils/Time.hpp"
#include "sl/components/RenderComponent.hpp"
#include "sl/components/EnabledComponent.hpp"
#include "sl/components/AnimationComponent.hpp"

#include "AnimationSystem.hpp"

namespace sl
{
	void AnimationSystem::event(ALLEGRO_EVENT* event, entt::DefaultRegistry& registry)
	{
		// Nothing on event for now.
	}

	void AnimationSystem::update(const double dt, entt::DefaultRegistry& registry)
	{
		// For each entity with an animation.
		registry.view<AnimationComponent, RenderComponent, EnabledComponent>()
			.each([&](entt::DefaultRegistry::entity_type entity, AnimationComponent& ac, RenderComponent& rc, EnabledComponent& ec)
		{
			// Only update animation if it is not paused.
			if (!ac.m_isPaused)
			{
				// Get currently active animation for the entity.
				Animation* animation = &(ac.m_animations[ac.m_activeAnimation]);

				// Increment timepassed for that animation frame.
				auto timepassed = (dt * animation->m_speed);
				ac.m_currentFrameTime += timepassed;

				// If the time passed is greater than the time allowed per frame...
				if (ac.m_currentFrameTime >= Time::milliseconds(animation->m_frames[animation->m_currentFrame].m_timePerFrame))
				{
					// ...reset current frame time and increment the current frame.
					ac.m_currentFrameTime = 0.0;
					animation->m_currentFrame++;

					// Make sure the new frame is not larger than the total amount of frames avaliable.
					// If it is, reset animation to beginning.
					// current frame is an index so we need to take 1 from total frames
					// arrays start at 0!
					if (animation->m_currentFrame > (animation->m_totalFrames - 1))
					{
						animation->m_currentFrame = 0;

						// And finally stop animation if it is not looped.
						if (!animation->m_isLooped)
						{
							ac.stop();
						}
					}

					// Then update the animation frame to render within the the render component by changing the textureid.
					rc.m_textureName = animation->m_frames[animation->m_currentFrame].m_frameTextureID;
				}
			}
		});
	}
}