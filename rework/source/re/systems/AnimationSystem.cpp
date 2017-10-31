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
		es.each<AnimationComponent, SpriteComponent>([dt](entityx::Entity entity, AnimationComponent& ac, SpriteComponent& sc)
		{
			if (!ac.m_isPaused)
			{
				auto timepassed = (dt * ac.m_animations.at(ac.m_activeAnimation).m_speed);
				ac.m_currentFrameTime += timepassed;
				
				if (ac.m_currentFrameTime >= Time::milliseconds(ac.m_animations.at(ac.m_activeAnimation).m_timePerFrame))
				{
					ac.m_currentFrameTime = 0.0;
					ac.m_animations.at(ac.m_activeAnimation).m_currentFrame++;

					if (ac.m_animations.at(ac.m_activeAnimation).m_currentFrame > (ac.m_animations.at(ac.m_activeAnimation).m_totalFrames - 1))
					{
						ac.m_animations.at(ac.m_activeAnimation).m_currentFrame = 0;

						if (!ac.m_animations.at(ac.m_activeAnimation).m_isLooped)
						{
							ac.stop();
						}
					}

					sc.m_spriteName = ac.m_animations.at(ac.m_activeAnimation).m_frames[ac.m_animations.at(ac.m_activeAnimation).m_currentFrame];

				}
			}
		});
	}
}