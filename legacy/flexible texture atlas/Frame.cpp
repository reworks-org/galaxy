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
		    : m_region {0.0f, 0.0f, 0.0f, 0.0f}, m_time_per_frame {0.0}
		{
		}

		Frame::Frame(const graphics::fRect& region, const double time_per_frame) noexcept
		    : m_region {region}, m_time_per_frame {0.0}
		{
		}

		Frame::Frame(const nlohmann::json& json)
		{
			std::string name = json.at("name");
			m_region         = SL_HANDLE.atlas()->get_tex_info(name).m_region;
			m_time_per_frame = json.at("time-per-frame");
		}
	} // namespace graphics
} // namespace galaxy