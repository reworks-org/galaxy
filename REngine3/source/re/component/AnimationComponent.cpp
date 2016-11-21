//
//  AnimationComponent.cpp
//  REngine3
//
//  Created by reworks on 16/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//  Code modified from: https://github.com/miguelmartin75/anax/blob/master/examples/common/include/Components/AnimationComponent.hpp
//  See original file for details.

#include "AnimationComponent.hpp"

namespace re
{
	AnimationState::AnimationState()
	{
	}

	AnimationState::AnimationState(sol::table& table)
	{
		m_frameRate = table.get<unsigned int>("fps");
		m_startPosition.x = table.get<int>("firstTextureFrameX");
		m_startPosition.y = table.get<int>("firstTextureFrameY");;
		m_frameAmount.x = table.get<int>("frameAmount");
		m_frameAmount.y = table.get<int>("frameAmount");
	}

	AnimationComponent::AnimationComponent()
		:m_currentFrame(0, 0), m_isPlaying(false), m_repeat(false)
	{
	}

	AnimationComponent::~AnimationComponent()
	{
		m_states.clear();
	}

	void AnimationComponent::Init(sol::table& table)
	{
		m_frameSize.x = table.get<int>("frameWidth");
		m_frameSize.y = table.get<int>("frameHeight");
		m_isPlaying = table.get<bool>("isPlaying");
		m_repeat = table.get<bool>("isRepeated");

		sol::table states = table.get<sol::table>("states");

		// Get key-value pairs from table
		std::map<std::string, sol::table> m_keyValuePair;
		states.for_each([&](std::pair<sol::object, sol::object> pair) {
			m_keyValuePair.insert({ pair.first.as<std::string>(), pair.second.as<sol::table>() });
		});

		for (auto& kvp : m_keyValuePair)
		{
			m_states[kvp.first] = AnimationState(kvp.second);
		}

		
		m_playingState = table.get<std::string>("defaultState");
		m_isPlaying = table.get<bool>("startPlaying");
	}

	void AnimationComponent::Play(const std::string& state)
	{
		m_playingState = state;
		m_isPlaying = true;
	}

	void AnimationComponent::Reset()
	{
		m_currentFrame.x = 0;
		m_currentFrame.y = 0;
	}

	void AnimationComponent::Pause()
	{
		m_isPlaying = false;
	}

	void AnimationComponent::Unpause()
	{
		m_isPlaying = true;
	}

	void AnimationComponent::EnableRepeat()
	{
		m_repeat = true;
	}

	void AnimationComponent::DisableRepeat()
	{
		m_repeat = false;
	}

	void AnimationComponent::ChangeAnimation(const std::string& stateName)
	{
		m_playingState = stateName;
		Reset();
	}

	void AnimationComponent::Stop()
	{
		Pause();
		Reset();
	}
}