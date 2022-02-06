///
/// Frame.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/resource/TextureBook.hpp"

#include "Frame.hpp"

namespace galaxy
{
	namespace graphics
	{
		Frame::Frame() noexcept
			: Serializable {this}
			, m_time_per_frame {0.0}
			, m_region {0.0f, 0.0f, 0.0f, 0.0f}
		{
		}

		Frame::Frame(const math::Rect<float>& region, const double time_per_frame) noexcept
			: Serializable {this}
			, m_time_per_frame {time_per_frame}
			, m_region {region}
		{
		}

		Frame::Frame(const nlohmann::json& json)
			: Serializable {this}
			, m_time_per_frame {0.0}
			, m_region {0.0f, 0.0f, 0.0f, 0.0f}
		{
			deserialize(json);
		}

		void Frame::set_region(std::string_view region)
		{
			const auto res = SL_HANDLE.texturebook()->search(region);
			if (res != std::nullopt)
			{
				m_id     = static_cast<std::string>(region);
				m_region = res.value().m_region;
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to find texture region for frame: {0}.", region);
			}
		}

		const math::Rect<float>& Frame::get_region() const noexcept
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