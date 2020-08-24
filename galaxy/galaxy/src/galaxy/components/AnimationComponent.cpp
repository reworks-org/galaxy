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
	    : m_abs {}
	{
	}

	AnimationComponent::AnimationComponent(const nlohmann::json& json)
	    : m_abs {}
	{
		m_abs.set_opacity(json.at("opacity"));

		auto anim_array = json.at("animations");
		for (const auto& anim : anim_array)
		{
			std::string name = anim.at("name");
			auto frame_array = json.at("frames");

			std::vector<qs::Frame> frames;
			frames.reserve(frame_array.size());
			for (const auto& frame : frame_array)
			{
				frames.emplace_back(frame.at("texture-x"), frame.at("texture-y"), frame.at("time-per-frame"));
			}

			m_abs.add_animation(name, name, anim.at("looping"), anim.at("speed"), frames);
		}

		m_abs.set_animation(json.at("default-anim"));
		auto* frame = m_abs.get_cur_animation()->get_current_frame();

		m_abs.create({frame->get_x(), frame->get_y(), json.at("w"), json.at("h")}, 1);
		m_abs.set_pos(json.at("x"), json.at("y"));

		m_abs.play();
	}

	AnimationComponent::AnimationComponent(AnimationComponent&& ac)
	{
		this->m_abs = std::move(ac.m_abs);
	}

	AnimationComponent& AnimationComponent::operator=(AnimationComponent&& ac)
	{
		if (this != &ac)
		{
			this->m_abs = std::move(ac.m_abs);
		}
		
		return *this;
	}
} // namespace galaxy