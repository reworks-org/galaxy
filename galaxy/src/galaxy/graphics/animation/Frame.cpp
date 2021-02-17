///
/// Frame.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/res/TextureAtlas.hpp"

#include "Frame.hpp"

namespace galaxy
{
	namespace graphics
	{
		Frame::Frame() noexcept
		    : Serializable {this}, m_id {""}, m_region {0.0f, 0.0f, 0.0f, 0.0f}, m_time_per_frame {0.0}
		{
		}

		Frame::Frame(const graphics::fRect& region, const double time_per_frame) noexcept
		    : Serializable {this}, m_id {""}, m_region {region}, m_time_per_frame {0.0}
		{
		}

		Frame::Frame(const nlohmann::json& json)
		    : Serializable {this}, m_id {""}, m_time_per_frame {0.0}
		{
			deserialize(json);
		}

		void Frame::set_region(std::string_view region)
		{
			m_id     = region;
			m_region = SL_HANDLE.atlas()->get_region(m_id);
		}

		const graphics::fRect& Frame::get_region() const noexcept
		{
			return m_region;
		}

		const std::string& Frame::get_region_id() const noexcept
		{
			return m_id;
		}

		nlohmann::json Frame::serialize()
		{
			nlohmann::json json    = "{}"_json;
			json["name"]           = m_id;
			json["time-per-frame"] = m_time_per_frame;

			return json;
		}

		void Frame::deserialize(const nlohmann::json& json)
		{
			set_region(json.at("name"));
			m_time_per_frame = json.at("time-per-frame");
		}
	} // namespace graphics
} // namespace galaxy