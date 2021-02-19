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
	namespace map
	{
		Frame::Frame()
		    : m_duration {0}, m_tile_id {0}
		{
		}

		Frame::Frame(const nlohmann::json& json)
		    : m_duration {0}, m_tile_id {0}
		{
			parse(json);
		}

		void Frame::parse(const nlohmann::json& json)
		{
			if (json.count("duration") > 0)
			{
				m_duration = json.at("duration");
			}

			if (json.count("tileid") > 0)
			{
				m_tile_id = json.at("tileid");
			}
		}

		const int Frame::get_duration() const
		{
			return m_duration;
		}

		const int Frame::get_tile_id() const
		{
			return m_tile_id;
		}
	} // namespace map
} // namespace galaxy