///
/// AnimationComponent.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "AnimationComponent.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	AnimationComponent::AnimationComponent()
	    : m_paused {true}, m_time_spent_on_frame {0}, m_active_animation {"null"}
	{
	}

	AnimationComponent::AnimationComponent(const nlohmann::json& json)
	    : m_paused {true}, m_time_spent_on_frame {0}
	{
		m_active_animation = json.at("default-anim");
		auto anim_array    = json.at("animations");
		for (const auto& anim : anim_array)
		{
			std::string name = anim.at("name");
			auto frame_array = json.at("frames");

			std::vector<qs::Frame> frames;
			frames.reserve(frame_array.size());
			for (const auto& frame : frame_array)
			{
				frames.emplace_back(frame.at("x"), frame.at("y"), frame.at("time-per-frame"));
			}

			m_animations.emplace(name, name, anim.at("looping"), anim.at("speed"), frames);
		}
	}

	AnimationComponent::~AnimationComponent()
	{
		m_animations.clear();
	}

	void AnimationComponent::set_animation(std::string_view animation)
	{
		if (m_active_animation != "null")
		{
			m_animations[m_active_animation].restart();
		}

		m_active_animation = static_cast<std::string>(animation);
		m_animations[m_active_animation].restart();
	}

	void AnimationComponent::play()
	{
		m_paused = false;
	}

	void AnimationComponent::play(std::string_view animation)
	{
		m_paused = false;
		set_animation(animation);
	}

	void AnimationComponent::pause()
	{
		m_paused = true;
	}

	void AnimationComponent::stop()
	{
		m_time_spent_on_frame = 0;
		m_paused              = true;
		m_animations[m_active_animation].restart();
	}
} // namespace galaxy