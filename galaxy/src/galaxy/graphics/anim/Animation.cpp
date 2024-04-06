///
/// Animation.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/error/Log.hpp"
#include "galaxy/scripting/JSON.hpp"

#include "Animation.hpp"

namespace galaxy
{
	namespace graphics
	{
		Animation::Animation()
			: m_speed {1.0}
			, m_index {0}
		{
		}

		Animation::Animation(Animation&& a)
		{
			this->m_speed  = a.m_speed;
			this->m_name   = std::move(a.m_name);
			this->m_index  = a.m_index;
			this->m_frames = std::move(a.m_frames);
		}

		Animation& Animation::operator=(Animation&& a)
		{
			if (this != &a)
			{
				this->m_speed  = a.m_speed;
				this->m_name   = std::move(a.m_name);
				this->m_index  = a.m_index;
				this->m_frames = std::move(a.m_frames);
			}

			return *this;
		}

		Animation::Animation(const Animation& a)
		{
			this->m_speed  = a.m_speed;
			this->m_name   = a.m_name;
			this->m_index  = a.m_index;
			this->m_frames = a.m_frames;
		}

		Animation& Animation::operator=(const Animation& a)
		{
			if (this != &a)
			{
				this->m_speed  = a.m_speed;
				this->m_name   = a.m_name;
				this->m_index  = a.m_index;
				this->m_frames = a.m_frames;
			}

			return *this;
		}

		Animation::~Animation()
		{
		}

		bool Animation::load(const std::string& file)
		{
			auto json = json::read(file);
			if (!json.empty())
			{
				m_name = std::filesystem::path(file).filename().string();

				set_frames(json);
				return true;
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Tried to set animation to empty json frames: {0}.", file);
				return false;
			}
		}

		void Animation::set_frames(const nlohmann::json& json)
		{
			for (const auto& obj : json)
			{
				Frame frame {
					.m_bounds   = {obj.at("x"), obj.at("y"), obj.at("width"), obj.at("height")},
					.m_duration = obj.at("duration")
                };

				m_frames.emplace_back(frame);
			}
		}

		void Animation::set_frames(const meta::vector<Frame>& frames)
		{
			m_frames = frames;
		}

		void Animation::next()
		{
			m_index++;

			if (m_index >= m_frames.size())
			{
				m_index = 0;
			}
		}

		void Animation::prev()
		{
			m_index--;

			if (m_index < 0)
			{
				m_index = m_frames.size() - 1;
			}
		}

		void Animation::restart()
		{
			m_index = 0;
		}

		Frame& Animation::current()
		{
			return m_frames[m_index];
		}

		std::size_t Animation::total() const
		{
			return m_frames.size();
		}

		std::size_t Animation::index() const
		{
			return m_index;
		}

		meta::vector<Frame>& Animation::frames()
		{
			return m_frames;
		}
	} // namespace graphics
} // namespace galaxy
