//
//  AnimationSystem.cpp
//  rework
//
//  Created by reworks on 10/11/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include "re/utils/Time.hpp"
#include "re/components/SpriteComponent.hpp"
#include "re/components/AnimationComponent.hpp"

#include "AnimationSystem.hpp"

namespace re
{
	AnimationSystem::AnimationSystem()
	{
	}

	AnimationSystem::~AnimationSystem()
	{
	}

	void AnimationSystem::update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt)
	{
		es.each<AnimationComponent, SpriteComponent>([dt](entityx::Entity& entity, AnimationComponent& ac, SpriteComponent& sc)
		{
			if (!ac.m_isPaused)
			{
				Animation* animation = &(ac.m_animations.at(ac.m_activeAnimation));

				auto timepassed = (dt * animation->m_speed);
				ac.m_currentFrameTime += timepassed;

				if (ac.m_currentFrameTime >= Time::milliseconds(animation->m_timePerFrame))
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
		});
	}
}