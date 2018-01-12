///
///  Animation.cpp
///  starlight
///
///  Created by reworks on 31/10/2017.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#include <map>

#include "sol2/sol.hpp"
#include "loguru/loguru.hpp"
#include "imgui/imgui_impl_a5.h"

#include "Animation.hpp"

namespace sl
{
	Animation::Animation(sol::table& table)
		:m_currentFrame(0)
	{
		m_frames.clear();

		m_isLooped = table.get<bool>("looped");
		m_speed = table.get<float>("speed");
		m_timePerFrame = table.get<std::int32_t>("timePerFrame");
		m_totalFrames = table.get<unsigned int>("totalFrames");

		sol::table frames = table.get<sol::table>("frames");
		m_frames.resize(m_totalFrames);
		
		if (!frames.empty())
		{
			frames.for_each([&](std::pair<sol::object, sol::object> pair)
			{
				m_frames[pair.first.as<int>()] = pair.second.as<std::string>();
			});
		}
		else
		{
			LOG_S(WARNING) << "Tried to load animation with no frames!";
		}
	}

	void Animation::debug()
	{
		ImGui::Checkbox("Is Looped", &m_isLooped);

		ImGui::Spacing();
		ImGui::InputFloat("Speed", &m_speed, 0.1, 0.1);

		ImGui::Spacing();
		ImGui::InputInt("Time Per Frame", &m_timePerFrame, 10, 20);
	}
}