//
//  AnimationComponent.cpp
//  REngine3
//
//  Created by reworks on 16/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
// 
// Code adapted from here: https://github.com/SFML/SFML/wiki/Source:-AnimationComponent

#include <map>

#include <SFML/Graphics/Rect.hpp>

#include "re/utility/Log.hpp"
#include "re/scripting/sol2/sol.hpp"

#include "AnimationComponent.hpp"

namespace re
{
	AnimationComponent::AnimationComponent()
	{
        m_isLooped = false;
        m_isPaused = false;
        m_frameTime = sf::Time::Zero;
        m_animations.clear();
        m_currentTime = sf::Time::Zero;
        m_currentFrame = 0;
        m_activeAnimation = "";
	}

	AnimationComponent::~AnimationComponent()
	{
		m_animations.clear();
	}

	void AnimationComponent::init(sol::table& table)
	{
		m_frameTime = sf::milliseconds(table.get<int>("speed"));
		m_currentFrame = 0;
		m_isPaused = table.get<bool>("isPaused");
		m_isLooped = table.get<bool>("isLooped");

		changeAnimation(table.get<std::string>("defaultAnim"));

		m_animations.clear();
		sol::table animTable = table.get<sol::table>("Animations");

		std::map<std::string, sol::table> m_keyValuePairAnimations;
		animTable.for_each([&](std::pair<sol::object, sol::object> pair) {
			m_keyValuePairAnimations.insert({ pair.first.as<std::string>(), pair.second.as<sol::table>() });
		});
        
		RE_REVERSE_ASSERT(m_keyValuePairAnimations.empty(), "Tried to load animation with no frames", "AnimationComponent::init", "AnimationComponent.cpp", 47);
        
		for (auto& it : m_keyValuePairAnimations)
		{
			std::map<std::string, sol::table> m_keyValuePairFrames;
			it.second.for_each([&](std::pair<sol::object, sol::object> pair) {
				m_keyValuePairFrames.insert({ pair.first.as<std::string>(), pair.second.as<sol::table>() });
			});

			std::vector<sf::IntRect> tempvector;

			for (auto& frames : m_keyValuePairFrames)
			{
				sf::IntRect temp(frames.second.get<int>("x"), frames.second.get<int>("y"), frames.second.get<int>("w"), frames.second.get<int>("h"));
				tempvector.push_back(temp);
			}

			m_animations.emplace(it.first, tempvector);
			tempvector.clear();
		}
	}

	void AnimationComponent::debugFunction(sol::table& table)
	{
	}

	void AnimationComponent::changeAnimation(const std::string& animation)
	{
		m_activeAnimation = animation;
		m_currentFrame = 0;
		m_currentTime = sf::Time::Zero;
	}

	void AnimationComponent::play()
	{
		m_isPaused = false;
	}

	void AnimationComponent::play(const std::string& animation)
	{
		if (m_activeAnimation != animation)
		{
			changeAnimation(animation);
		}
			
		play();
	}

	void AnimationComponent::pause()
	{
		m_isPaused = true;
	}

	void AnimationComponent::stop()
	{
		m_isPaused = true;
		m_currentFrame = 0;
		m_currentTime = sf::Time::Zero;
	}

	bool AnimationComponent::isPaused() const
	{
		return m_isPaused;
	}
}
