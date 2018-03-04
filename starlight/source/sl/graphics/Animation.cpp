///
/// Animation.cpp
/// starlight
///
/// Created by reworks on 31/10/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include <map>

#include "sl/libs/sol2/sol.hpp"
#include "sl/libs/loguru/loguru.hpp"

#include "Animation.hpp"

namespace sl
{
	Animation::Animation(sol::table& table)
		:m_currentFrame(0)
	{
		m_frames.clear();

		m_isLooped = table.get<bool>("looped");
		m_speed = table.get<float>("speed");
		m_timePerFrame = table.get<std::uint32_t>("timePerFrame");
		m_totalFrames = table.get<unsigned int>("totalFrames");

		sol::table frames = table.get<sol::table>("frames");
		m_frames.resize(m_totalFrames);
		
		if (!frames.empty())
		{
			frames.for_each([&](std::pair<sol::object, sol::object> pair)
			{
				m_frames[pair.first.as<int>()] = pair.second.as<std::string_view>();
			});
		}
		else
		{
			LOG_S(WARNING) << "Tried to load animation with no frames!";
		}
	}

	Animation::Animation(bool isLooped, float speed, std::uint32_t timePerFrame, unsigned int totalFrames, unsigned int currentFrame, const std::vector<std::string_view>& frames)
		:m_isLooped(isLooped), m_speed(speed), m_timePerFrame(timePerFrame), m_totalFrames(totalFrames), m_currentFrame(currentFrame), m_frames(frames)
	{
	}
}