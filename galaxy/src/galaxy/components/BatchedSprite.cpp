///
/// BatchedSprite.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <algorithm>

#include <nlohmann/json.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/res/TextureAtlas.hpp"

#include "BatchedSprite.hpp"

namespace galaxy
{
	namespace components
	{
		BatchedSprite::BatchedSprite() noexcept
		    : Serializable {this}, m_opacity {1.0f}, m_region {0.0f, 0.0f, 0.0f, 0.0f}, m_offset {0}, m_z_level {0}, m_custom_wh {0.0f, 0.0f}
		{
		}

		BatchedSprite::BatchedSprite(const nlohmann::json& json)
		    : Serializable {this}, m_opacity {1.0f}, m_region {0.0f, 0.0f, 0.0f, 0.0f}, m_offset {0}, m_z_level {0}, m_custom_wh {0.0f, 0.0f}
		{
			deserialize(json);
		}

		BatchedSprite::BatchedSprite(BatchedSprite&& bs) noexcept
		    : Serializable {this}
		{
			this->m_id        = bs.m_id;
			this->m_offset    = bs.m_offset;
			this->m_opacity   = bs.m_opacity;
			this->m_region    = std::move(bs.m_region);
			this->m_custom_wh = std::move(bs.m_custom_wh);
		}

		BatchedSprite& BatchedSprite::operator=(BatchedSprite&& bs) noexcept
		{
			if (this != &bs)
			{
				this->m_id        = bs.m_id;
				this->m_offset    = bs.m_offset;
				this->m_opacity   = bs.m_opacity;
				this->m_region    = std::move(bs.m_region);
				this->m_custom_wh = std::move(bs.m_custom_wh);
			}

			return *this;
		}

		BatchedSprite::~BatchedSprite() noexcept
		{
			m_offset = 0;
		}

		void BatchedSprite::create(const graphics::fRect& region, float opacity)
		{
			m_region  = region;
			m_opacity = std::clamp(opacity, 0.0f, 1.0f);
		}

		void BatchedSprite::create(std::string_view texture_atlas_id, float opacity)
		{
			m_region  = SL_HANDLE.atlas()->get_region(texture_atlas_id);
			m_opacity = std::clamp(opacity, 0.0f, 1.0f);
		}

		void BatchedSprite::set_region(std::string_view region)
		{
			m_id     = region;
			m_region = SL_HANDLE.atlas()->get_region(m_id);

			m_custom_wh = {0.0f, 0.0f};
		}

		void BatchedSprite::set_opacity(const float opacity) noexcept
		{
			m_opacity = std::clamp(opacity, 0.0f, 1.0f);
		}

		void BatchedSprite::set_custom_width(const float width) noexcept
		{
			m_custom_wh.x    = width;
			m_region.m_width = width;
		}

		void BatchedSprite::set_custom_height(const float height) noexcept
		{
			m_custom_wh.y     = height;
			m_region.m_height = height;
		}

		const float BatchedSprite::get_opacity() const noexcept
		{
			return m_opacity;
		}

		const int BatchedSprite::get_width() const noexcept
		{
			return m_region.m_width;
		}

		const int BatchedSprite::get_height() const noexcept
		{
			return m_region.m_height;
		}

		const graphics::fRect& BatchedSprite::get_region() const noexcept
		{
			return m_region;
		}

		nlohmann::json BatchedSprite::serialize()
		{
			nlohmann::json json      = "{}"_json;
			json["texture-atlas-id"] = m_id;
			json["opacity"]          = m_opacity;

			if (m_custom_wh.x != 0.0f)
			{
				json["custom-width"] = m_custom_wh.x;
			}

			if (m_custom_wh.y != 0.0f)
			{
				json["custom-height"] = m_custom_wh.y;
			}

			return json;
		}

		void BatchedSprite::deserialize(const nlohmann::json& json)
		{
			set_region(json.at("texture-atlas-id"));
			m_opacity = std::clamp(json.at("opacity").get<float>(), 0.0f, 1.0f);

			if (json.count("custom-width") > 0)
			{
				m_custom_wh.x = json.at("custom-width");
			}

			if (json.count("custom-height") > 0)
			{
				m_custom_wh.y = json.at("custom-height");
			}
		}
	} // namespace components
} // namespace galaxy