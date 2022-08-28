///
/// Animation.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "Animation.hpp"

namespace galaxy
{
	namespace graphics
	{
		Animation::Animation() noexcept
			: Serializable {}
			, m_active_frame {nullptr}
			, m_name {"null"}
			, m_looping {false}
			, m_speed {0.0}
			, m_total_frames {0}
			, m_current_frame_index {0}
		{
		}

		Animation::Animation(std::string_view name, bool looping, const double speed, std::span<Frame> frames) noexcept
			: Serializable {}
			, m_active_frame {nullptr}
			, m_name {name}
			, m_looping {looping}
			, m_speed {speed}
			, m_total_frames {frames.size()}
			, m_current_frame_index {0}
		{
			m_frames.assign(frames.begin(), frames.end());
		}

		Animation::Animation(const nlohmann::json& json)
			: Serializable {}
			, m_active_frame {nullptr}
			, m_name {"null"}
			, m_looping {false}
			, m_speed {0.0}
			, m_total_frames {0}
			, m_current_frame_index {0}
		{
			deserialize(json);
		}

		Animation::Animation(Animation&& a) noexcept
		{
			this->m_active_frame        = a.m_active_frame;
			this->m_name                = std::move(a.m_name);
			this->m_looping             = a.m_looping;
			this->m_speed               = a.m_speed;
			this->m_total_frames        = a.m_total_frames;
			this->m_current_frame_index = a.m_current_frame_index;
			this->m_frames              = std::move(a.m_frames);

			a.m_active_frame = nullptr;
		}

		Animation& Animation::operator=(Animation&& a) noexcept
		{
			if (this != &a)
			{
				this->m_active_frame        = a.m_active_frame;
				this->m_name                = std::move(a.m_name);
				this->m_looping             = a.m_looping;
				this->m_speed               = a.m_speed;
				this->m_total_frames        = a.m_total_frames;
				this->m_current_frame_index = a.m_current_frame_index;
				this->m_frames              = std::move(a.m_frames);

				a.m_active_frame = nullptr;
			}

			return *this;
		}

		Animation::Animation(const Animation& a) noexcept
		{
			this->m_active_frame        = a.m_active_frame;
			this->m_name                = std::move(a.m_name);
			this->m_looping             = a.m_looping;
			this->m_speed               = a.m_speed;
			this->m_total_frames        = a.m_total_frames;
			this->m_current_frame_index = a.m_current_frame_index;
			this->m_frames              = std::move(a.m_frames);
		}

		Animation& Animation::operator=(const Animation& a) noexcept
		{
			if (this != &a)
			{
				this->m_active_frame        = a.m_active_frame;
				this->m_name                = a.m_name;
				this->m_looping             = a.m_looping;
				this->m_speed               = a.m_speed;
				this->m_total_frames        = a.m_total_frames;
				this->m_current_frame_index = a.m_current_frame_index;
				this->m_frames              = a.m_frames;
			}

			return *this;
		}

		Animation::~Animation() noexcept
		{
			m_active_frame = nullptr;
			m_frames.clear();
		}

		Frame* Animation::next_frame() noexcept
		{
			m_current_frame_index++;
			if (m_current_frame_index >= m_frames.size())
			{
				if (m_looping)
				{
					m_current_frame_index = 0;
					m_active_frame        = &m_frames[m_current_frame_index];

					return m_active_frame;
				}
				else
				{
					m_current_frame_index = m_frames.size();
					m_active_frame        = &m_frames[m_current_frame_index - 1];
				}
			}
			else
			{
				m_active_frame = &m_frames[m_current_frame_index];
			}

			return m_active_frame;
		}

		void Animation::restart() noexcept
		{
			m_current_frame_index = 0;
			m_active_frame        = &m_frames[0];
		}

		Frame* Animation::current_frame() noexcept
		{
			return m_active_frame;
		}

		std::size_t Animation::current_frame_index() noexcept
		{
			return m_current_frame_index;
		}

		nlohmann::json Animation::serialize()
		{
			nlohmann::json json  = "{}"_json;
			json["name"]         = m_name;
			json["loop"]         = m_looping;
			json["speed"]        = m_speed;
			json["total_frames"] = m_total_frames;

			json["frames"] = nlohmann::json::object();
			for (std::size_t i = 0; i < m_frames.size(); i++)
			{
				json["frames"][std::to_string(i)] = m_frames[i].serialize();
			}

			return json;
		}

		void Animation::deserialize(const nlohmann::json& json)
		{
			m_active_frame        = nullptr;
			m_current_frame_index = 0;
			m_frames.clear();

			m_name         = json.at("name");
			m_looping      = json.at("loop");
			m_speed        = json.at("speed");
			m_total_frames = json.at("total_frames");

			const auto& frames = json.at("frames");
			for (const auto& [id, obj] : frames.items())
			{
				m_frames.emplace_back(Frame {obj});
			}
		}
	} // namespace graphics
} // namespace galaxy