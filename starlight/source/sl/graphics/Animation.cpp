///
/// Animation.cpp
/// starlight
///
/// Created by reworks on 31/10/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include <map>

#include "sol2/sol.hpp"
#include "loguru/loguru.hpp"

#include "Animation.hpp"

namespace sl
{
	Animation::Animation()
	{
		LOG_S(FATAL) << "Program attempted to create a default constructed Animation!";
	}

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
				m_frames[pair.first.as<int>()] = pair.second.as<std::string_view>();
			});
		}
		else
		{
			LOG_S(WARNING) << "Tried to load animation with no frames!";
		}
	}
}