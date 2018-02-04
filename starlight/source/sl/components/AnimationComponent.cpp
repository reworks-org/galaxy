///
/// AnimationComponent.cpp
/// starlight
///
/// Created by reworks on 16/08/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sol2/sol.hpp"
#include "loguru/loguru.hpp"

#include "AnimationComponent.hpp"

namespace sl
{
	AnimationComponent::AnimationComponent(const sol::table& table)
		:m_currentFrameTime(0.0)
	{
		m_isPaused = table.get<bool>("paused");
		m_activeAnimation = table.get<std::string_view>("defaultAnim");

		sol::table animTable = table.get<sol::table>("Animations");
		if (!animTable.empty())
		{
			animTable.for_each([&](std::pair<sol::object, sol::object> pair)
			{
				m_animations.emplace(pair.first.as<std::string_view>(), pair.second.as<sol::table>());
			});
		}
		else
		{
			LOG_S(WARNING) << "Tried to load AnimationComponent with no Animations!";
		}
	}

	AnimationComponent::~AnimationComponent()
	{
		m_animations.clear();
	}

	void AnimationComponent::changeAnimation(std::string_view animation)
	{
		m_animations[m_activeAnimation].m_currentFrame = 0;
		m_currentFrameTime = 0.0;
		m_activeAnimation = animation;
	}

	void AnimationComponent::play()
	{
		m_isPaused = false;
	}

	void AnimationComponent::play(std::string_view animation)
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
		m_animations[m_activeAnimation].m_currentFrame = 0;
		m_currentFrameTime = 0.0;
	}

	AnimationComponent& AnimationComponent::operator=(const AnimationComponent&)
	{
		return *this;
	}
}