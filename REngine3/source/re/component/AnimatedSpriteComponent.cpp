//
//  AnimatedSpriteComponent.cpp
//  REngine3
//
//  Created by reworks on 16/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <map>

#include "re/deps/sol2/sol.hpp"

#include "AnimatedSpriteComponent.hpp"

namespace re
{
	AnimatedSpriteComponent::AnimatedSpriteComponent()
	{
	}

	AnimatedSpriteComponent::~AnimatedSpriteComponent()
	{
		m_animations.clear();
	}

	void AnimatedSpriteComponent::Init(sol::table& table)
	{
		m_group = table.get<int>("group");

		// set up animatedsprite parent class.
		// using NULL over nullptr because this is what the original class uses.

		m_animation = NULL;
		m_frameTime = sf::seconds(table.get<float>("frameTime"));
		m_speed = table.get<float>("speed");
		m_currentFrame = 0;
		m_isPaused = table.get<bool>("isPaused");
		m_isLooped = table.get<bool>("isLooped");
		m_texture = NULL;

		m_animationSheetStream.open(table.get<std::string>("texture"));
		m_animationSheet.loadFromStream(m_animationSheetStream);

		SetActiveAnimation(table.get<std::string>("defaultAnim"));

		m_animations.clear();
		sol::table animTable = table.get<sol::table>("Animations");

		std::map<std::string, sol::table> m_keyValuePairAnimations;
		animTable.for_each([&](std::pair<sol::object, sol::object> pair) {
			m_keyValuePairAnimations.insert({ pair.first.as<std::string>(), pair.second.as<sol::table>() });
		});

		for (auto& it : m_keyValuePairAnimations)
		{
			std::map<std::string, sol::table> m_keyValuePairFrames;
			it.second.for_each([&](std::pair<sol::object, sol::object> pair) {
				m_keyValuePairFrames.insert({ pair.first.as<std::string>(), pair.second.as<sol::table>() });
			});

			Animation temp;
			temp.setSpriteSheet(m_animationSheet);

			for (auto& frames : m_keyValuePairFrames)
			{
				temp.addFrame(sf::IntRect(frames.second.get<int>("x"), frames.second.get<int>("y"), frames.second.get<int>("w"), frames.second.get<int>("h")));
			}

			m_animations.emplace(it.first, temp);
		}
	}

	void AnimatedSpriteComponent::SetActiveAnimation(const std::string & animation)
	{
		m_activeAnimation = animation;
	}

	void AnimatedSpriteComponent::Update(sf::Time dt, float x, float y)
	{
		setPosition(x, y);

		// from AnimatedSprite.hpp

		// if not paused and we have a valid animation
		if (!m_isPaused && m_animation)
		{
			// add delta time
			m_currentTime += dt;

			// if current time is bigger then the frame time advance one frame
			if (m_currentTime >= m_frameTime)
			{
				// reset time, but keep the remainder
				m_currentTime = sf::microseconds(m_currentTime.asMicroseconds() % m_frameTime.asMicroseconds());

				// get next Frame index
				if (m_currentFrame + 1 < m_animation->getSize())
					m_currentFrame++;
				else
				{
					// animation has ended
					m_currentFrame = 0; // reset to start

					if (!m_isLooped)
					{
						m_isPaused = true;
					}

				}

				// set the current frame, not reseting the time
				setFrame(m_currentFrame, false);
			}
		}
	}

	void AnimatedSpriteComponent::Play()
	{
		play(m_animations[m_activeAnimation]);
	}

	void AnimatedSpriteComponent::draw(sf::RenderTarget & target, sf::RenderStates states) const
	{
		// from AnimatedSprite.hpp
		states.transform *= getTransform();
		states.texture = m_texture;
		target.draw(m_vertices, 4, sf::Quads, states);
	}
}