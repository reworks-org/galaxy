//
//  AnimationComponent.cpp
//  rework
//
//  Created by reworks on 16/08/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//
// Code adapted from here: https://github.com/SFML/SFML/wiki/Source:-AnimationComponent

#include <map>

#include "re/utils/Log.hpp"
#include "re/utils/Time.hpp"
#include "imgui/imgui_impl_a5.h"

#include "AnimationComponent.hpp"

namespace re
{
	AnimationComponent::AnimationComponent(sol::table& table)
	:m_isLooped(false), m_isPaused(false), m_frameTime(0.0f), m_currentTime(0.0f), m_currentFrame(0), m_activeAnimation("")
	{
        m_animations.clear();

		m_frameTime = Time::milliseconds(table.get<std::int32_t>("speed"));
		m_currentFrame = 0;
		m_isPaused = table.get<bool>("isPaused");
		m_isLooped = table.get<bool>("isLooped");

		changeAnimation(table.get<std::string>("defaultAnim"));

		m_animations.clear();
		sol::table animTable = table.get<sol::table>("Animations");

		std::map<std::string, sol::table> kvp;
		animTable.for_each([&](std::pair<sol::object, sol::object> pair) {
			kvp.insert({ pair.first.as<std::string>(), pair.second.as<sol::table>() });
		});

		if (kvp.empty())
		{
			BOOST_LOG_TRIVIAL(warning) << "Tried to load animation with no frames!" << std::endl;
		}

		for (auto& it : kvp)
		{
			std::map<std::string, sol::table> kvf;
			it.second.for_each([&](std::pair<sol::object, sol::object> pair) {
				kvf.insert({ pair.first.as<std::string>(), pair.second.as<sol::table>() });
			});

			std::vector<Rect<int>> tempvector;

			for (auto& frames : kvf)
			{
				Rect<int> temp(frames.second.get<int>("x"), frames.second.get<int>("y"), frames.second.get<int>("w"), frames.second.get<int>("h"));
				tempvector.push_back(temp);
			}

			m_animations.emplace(it.first, tempvector);
			tempvector.clear();
		}
	}

	AnimationComponent::~AnimationComponent()
	{
		m_animations.clear();
	}

	void AnimationComponent::changeAnimation(const std::string& animation)
	{
		m_activeAnimation = animation;
		m_currentFrame = 0;
		m_currentTime = 0.0f;
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
		m_currentTime = 0.0f;
	}

	bool AnimationComponent::isPaused() const
	{
		return m_isPaused;
	}

	void AnimationComponent::debug()
	{
		ImGui::InputFloat("Time Per Frame", &m_frameTime);

		ImGui::Spacing();
		ImGui::InputInt("Current Frame", &m_currentFrame);

		ImGui::Spacing();
		ImGui::Checkbox("Is Looping", &m_isLooped);

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
}