///
/// Frame.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "Frame.hpp"

namespace galaxy
{
	namespace graphics
	{
		Frame::Frame()
			: Serializable {}
			, m_time_per_frame {0.0}
		{
		}

		Frame::Frame(const nlohmann::json& json)
			: Serializable {}
			, m_time_per_frame {0.0}
		{
			deserialize(json);
		}

		Frame::Frame(Frame&& f)
		{
			this->m_texture_id     = std::move(f.m_texture_id);
			this->m_time_per_frame = f.m_time_per_frame;
		}

		Frame& Frame::operator=(Frame&& f)
		{
			if (this != &f)
			{
				this->m_texture_id     = std::move(f.m_texture_id);
				this->m_time_per_frame = f.m_time_per_frame;
			}

			return *this;
		}

		Frame::Frame(const Frame& f)
		{
			this->m_texture_id     = f.m_texture_id;
			this->m_time_per_frame = f.m_time_per_frame;
		}

		Frame& Frame::operator=(const Frame& f)
		{
			if (this != &f)
			{
				this->m_texture_id     = f.m_texture_id;
				this->m_time_per_frame = f.m_time_per_frame;
			}

			return *this;
		}

		Frame::~Frame()
		{
		}

		nlohmann::json Frame::serialize()
		{
			nlohmann::json json    = "{}"_json;
			json["texture_id"]     = m_texture_id;
			json["time_per_frame"] = m_time_per_frame;

			return json;
		}

		void Frame::deserialize(const nlohmann::json& json)
		{
			m_texture_id     = json.at("texture_id");
			m_time_per_frame = json.at("time_per_frame");
		}
	} // namespace graphics
} // namespace galaxy