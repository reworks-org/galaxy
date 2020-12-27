///
/// AnimatedBatchSprite.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "AnimatedBatchSprite.hpp"

namespace galaxy
{
	namespace graphics
	{
		AnimatedBatchSprite::AnimatedBatchSprite()
		    : BatchedSprite {}, Animated {}
		{
		}

		AnimatedBatchSprite::AnimatedBatchSprite(const nlohmann::json& json)
		    : BatchedSprite {}, Animated {}
		{
			set_opacity(json.at("opacity"));

			auto anim_array = json.at("animations");
			for (const auto& anim : anim_array)
			{
				std::string name = anim.at("name");
				auto frame_array = json.at("frames");

				std::vector<Frame> frames;
				frames.reserve(frame_array.size());
				for (const auto& frame : frame_array)
				{
					frames.emplace_back(frame.at("texture-x"), frame.at("texture-y"), frame.at("time-per-frame"));
				}

				add_animation(name, name, anim.at("looping"), anim.at("speed"), frames);
			}

			set_animation(json.at("default-anim"));
			auto* frame = get_cur_animation()->get_current_frame();

			create({frame->get_x(), frame->get_y(), json.at("w"), json.at("h")}, 1);
			set_pos(json.at("x"), json.at("y"));

			play();
		}

		void AnimatedBatchSprite::update(const double dt)
		{
			if (!m_paused)
			{
				m_time_spent_on_frame += (dt * m_active_anim->get_speed());
				if (m_active_anim->get_current_frame()->get_time_per_frame() >= m_time_spent_on_frame)
				{
					m_time_spent_on_frame = 0;
					auto* new_frame       = m_active_anim->next_frame();

					update_region(new_frame->get_x(), new_frame->get_y(), 0.0f, 0.0f);
				}
			}
		}
	} // namespace graphics
} // namespace galaxy