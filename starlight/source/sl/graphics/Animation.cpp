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
	Animation::Animation()
	{
		LOG_S(FATAL) << "Tried to construct a default-initialized animation!";
	}

	Animation::Animation(sol::table& table)
		:m_currentFrame(0)
	{
		m_frames.clear();

		m_isLooped = table.get<bool>("looped");
		m_speed = table.get<float>("speed");
		m_totalFrames = table.get<unsigned int>("totalFrames");

		sol::table frames = table.get<sol::table>("frames");
		m_frames.resize(m_totalFrames);
		
		if (!frames.empty())
		{
			frames.for_each([&](std::pair<sol::object, sol::object> pair)
			{
				sol::table frameTable = pair.second.as<sol::table>();
				m_frames[pair.first.as<int>()] = AnimationFrame{ frameTable.get<std::uint32_t>("timePerFrame"), frameTable.get<std::string>("frameTextureID") };
			});
		}
		else
		{
			LOG_S(WARNING) << "Tried to load animation with no frames!";
		}
	}

	Animation::Animation(bool isLooped, float speed, unsigned int totalFrames, unsigned int currentFrame, const std::vector<AnimationFrame>& frames)
		:m_isLooped(isLooped), m_speed(speed), m_totalFrames(totalFrames), m_currentFrame(currentFrame), m_frames(frames)
	{
	}

	Animation::Animation(Animation&& animation)
	:m_isLooped(animation.m_isLooped), m_speed(animation.m_speed), m_totalFrames(animation.m_totalFrames), m_currentFrame(animation.m_currentFrame), m_frames(animation.m_frames)
	{
	}

	Animation::Animation(const Animation& animation)
	: m_isLooped(animation.m_isLooped), m_speed(animation.m_speed), m_totalFrames(animation.m_totalFrames), m_currentFrame(animation.m_currentFrame), m_frames(animation.m_frames)
	{
	}
}