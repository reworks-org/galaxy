///
/// Animation.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Animation.hpp"

namespace galaxy
{
	namespace graphics
	{
		Animation::Animation() noexcept
		    : Serializable {this}, m_active_frame {nullptr}, m_name {"null"}, m_looping {false}, m_speed {0.0}, m_total_frames {0}, m_current_frame_index {0}
		{
		}

		Animation::Animation(std::string_view name, bool looping, const double speed, std::span<Frame> frames) noexcept
		    : Serializable {this}, m_active_frame {nullptr}, m_name {name}, m_looping {looping}, m_speed {speed}, m_total_frames {frames.size()}, m_current_frame_index {0}, m_frames {frames.begin(), frames.end()}
		{
		}

		Animation::Animation(std::string_view name, const nlohmann::json& json)
		    : Serializable {this}, m_name {name}
		{
			deserialize(json);
		}

		Animation::~Animation() noexcept
		{
			m_frames.clear();
		}

		void Animation::restart() noexcept
		{
			m_current_frame_index = 0;
			m_active_frame        = &m_frames[0];
		}

		Frame* const Animation::next_frame()
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

		const std::string& Animation::get_name() const noexcept
		{
			return m_name;
		}

		const bool Animation::is_looping() const noexcept
		{
			return m_looping;
		}

		const double Animation::get_speed() const noexcept
		{
			return m_speed;
		}

		const std::size_t Animation::get_total_frames() const noexcept
		{
			return m_total_frames;
		}

		Frame* const Animation::get_current_frame() const noexcept
		{
			return m_active_frame;
		}

		const std::vector<Frame>& Animation::get_frames() const noexcept
		{
			return m_frames;
		}

		nlohmann::json Animation::serialize()
		{
			nlohmann::json json  = "{}"_json;
			json["loop"]         = m_looping;
			json["speed"]        = m_speed;
			json["total-frames"] = m_total_frames;

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

			m_looping      = json.at("loop");
			m_speed        = json.at("speed");
			m_total_frames = json.at("total-frames");

			const auto frames = json.at("frames");
			for (const auto& [id, obj] : frames.items())
			{
				m_frames.emplace_back(Frame {obj});
			}
		}
	} // namespace graphics
} // namespace galaxy