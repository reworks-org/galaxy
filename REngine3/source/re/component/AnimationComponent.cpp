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
#include "re/debug/imgui/imgui-SFML.h"

#include "AnimationComponent.hpp"

namespace re
{
	AnimationComponent::AnimationComponent()
	:m_isLooped(false), m_isPaused(false), m_frameTime(sf::Time::Zero), m_currentTime(sf::Time::Zero), m_currentFrame(0), m_activeAnimation("")
	{
        m_animations.clear();
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

	void AnimationComponent::debugFunction(sol::table& table, const std::string& curEntityName)
	{
		static int frameTime = static_cast<int>(m_frameTime.asMilliseconds());
		static bool looped = m_isLooped;
		static std::string originalEntityName = curEntityName;
		static std::vector<std::string> anims;
		static int index = 0;
		static bool doneOnce = false;

		if (originalEntityName != curEntityName)
		{
			frameTime = static_cast<int>(m_frameTime.asMilliseconds());
			looped = m_isLooped;
			originalEntityName = curEntityName;
			
			anims.clear();
			for (auto& it : m_animations)
			{
				anims.push_back(it.first);
			}

			auto it = std::find(anims.begin(), anims.end(), m_activeAnimation);
			if (it != anims.end())
			{
				index = std::distance(anims.begin(), it);
			}
		}
		
		if (!doneOnce)
		{
			for (auto& it : m_animations)
			{
				anims.push_back(it.first);
			}

			doneOnce = true;
		}

		// change animation
		// play, pause, stop animation

		ImGui::Spacing();
		if (ImGui::InputInt("Time Per Frame", &frameTime, 10))
		{
			if (frameTime < 0) frameTime = 0;

			m_frameTime = sf::milliseconds(frameTime);
		}

		ImGui::Spacing();
		ImGui::Checkbox("Is Looping", &looped);
		m_isLooped = looped;

		ImGui::Spacing();
		if (ImGui::SFML::Combo("Change Animation", &index, anims))
		{
			changeAnimation(anims[index]);
		}
		

		ImGui::Spacing();
		if (ImGui::Button("Play animation"))
		{
			play();
		}

		ImGui::Spacing();
		if (ImGui::Button("Pause animation"))
		{
			pause();
		}

		ImGui::Spacing();
		if (ImGui::Button("Stop Animation"))
		{
			stop();
		}
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
