//
//  AnimationSystem.cpp
//  REngine3
//
//  Created by reworks on 10/11/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//  Code modified from: https://github.com/miguelmartin75/anax/blob/master/examples/common/include/Systems/AnimationSystem.hpp
//  See original file for details.

#include "re/component/SpriteComponent.hpp"
#include "re/component/AnimationComponent.hpp"

#include "AnimationSystem.hpp"

namespace re
{
	AnimationSystem::~AnimationSystem()
	{
		m_entitys.clear();
	}

	void AnimationSystem::AutoSubmit(World* world)
	{
	}

	void AnimationSystem::AddEntity(Entity* e)
	{
	}

	void AnimationSystem::RemoveEntity(const std::string& name)
	{
	}

	void AnimationSystem::Update(sf::Time dt)
	{
		for (auto& e : m_entitys)
		{
			auto animation = e.second->Get<AnimationComponent>();
			auto sprite = e.second->Get<SpriteComponent>();

			AnimationState* animationState = nullptr;
			if (!animation->m_playingState.empty())
			{
				animationState = &animation->m_states[animation->m_playingState];
			}

			if (animation->m_isPlaying && animationState)
			{
				animation->m_frameAccumulator += dt.asMilliseconds() * (animationState->m_frameRate == 0 ? m_fps : animationState->m_frameRate);
				animation->m_currentFrame.x = (int)animation->m_frameAccumulator;

				if (animation->m_currentFrame.x >= animationState->m_frameAmount.x)
				{
					// go to the next row (if necessary)
					if (animationState->m_frameAmount.y)
					{
						if (animation->m_currentFrame.y >= animationState->m_frameAmount.y)
						{
							animation->m_currentFrame.y = 0;
						}
						else
						{
							++animation->m_currentFrame.y;
						}
					}

					// reset the animation
					animation->m_currentFrame.x = 0;
					animation->m_frameAccumulator = 0;

					animation->m_isPlaying = animation->m_repeat;
				}
			}

			if (animationState)
			{
				sf::IntRect rect(sf::Vector2i(animationState->m_startPosition.x + animation->m_frameSize.x * (int)animation->m_currentFrame.x,
					animationState->m_startPosition.y + animation->m_frameSize.y * (int)animation->m_currentFrame.y),
					sf::Vector2i(animation->m_frameSize));

				sprite->setTextureRect(rect);
			}
		}
	}

	void AnimationSystem::SetFPS(unsigned int fps)
	{
		m_fps = fps;
	}

	void AnimationSystem::Clean()
	{
		m_entitys.clear();
	}
}