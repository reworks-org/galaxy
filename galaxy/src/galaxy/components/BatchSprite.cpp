///
/// BatchSprite.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <algorithm>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/resource/TextureAtlas.hpp"

#include "BatchSprite.hpp"

namespace galaxy
{
	namespace components
	{
		BatchSprite::BatchSprite() noexcept
		    : Serializable {this}, m_opacity {1.0f}, m_region {0.0f, 0.0f, 0.0f, 0.0f}, m_offset {0}, m_z_level {0}, m_custom_wh {0.0f, 0.0f}
		{
		}

		BatchSprite::BatchSprite(const nlohmann::json& json)
		    : Serializable {this}, m_opacity {1.0f}, m_region {0.0f, 0.0f, 0.0f, 0.0f}, m_offset {0}, m_z_level {0}, m_custom_wh {0.0f, 0.0f}
		{
			deserialize(json);
		}

		BatchSprite::BatchSprite(BatchSprite&& bs) noexcept
		    : Serializable {this}
		{
			this->m_id        = bs.m_id;
			this->m_opacity   = bs.m_opacity;
			this->m_region    = std::move(bs.m_region);
			this->m_custom_wh = std::move(bs.m_custom_wh);
			this->m_offset    = bs.m_offset;
			this->m_z_level   = bs.m_z_level;
			this->m_vertexs   = std::move(bs.m_vertexs);
		}

		BatchSprite& BatchSprite::operator=(BatchSprite&& bs) noexcept
		{
			if (this != &bs)
			{
				this->m_id        = bs.m_id;
				this->m_opacity   = bs.m_opacity;
				this->m_region    = std::move(bs.m_region);
				this->m_custom_wh = std::move(bs.m_custom_wh);
				this->m_offset    = bs.m_offset;
				this->m_z_level   = bs.m_z_level;
				this->m_vertexs   = std::move(bs.m_vertexs);
			}

			return *this;
		}

		BatchSprite::~BatchSprite() noexcept
		{
			m_offset = 0;
		}

		void BatchSprite::create(const graphics::fRect& region, float opacity)
		{
			m_region  = region;
			m_opacity = std::clamp(opacity, 0.0f, 1.0f);
		}

		void BatchSprite::create(std::string_view texture_atlas_id, float opacity)
		{
			set_region(texture_atlas_id);
			m_opacity = std::clamp(opacity, 0.0f, 1.0f);
		}

		void BatchSprite::set_region(std::string_view region)
		{
			m_id     = region;
			m_region = SL_HANDLE.atlas()->get_region(m_id);

			m_custom_wh = {0.0f, 0.0f};
		}

		void BatchSprite::set_opacity(const float opacity) noexcept
		{
			m_opacity = std::clamp(opacity, 0.0f, 1.0f);
		}

		void BatchSprite::set_custom_width(const float width) noexcept
		{
			m_custom_wh.x    = width;
			m_region.m_width = width;
		}

		void BatchSprite::set_custom_height(const float height) noexcept
		{
			m_custom_wh.y     = height;
			m_region.m_height = height;
		}

		const float BatchSprite::get_opacity() const noexcept
		{
			return m_opacity;
		}

		const int BatchSprite::get_width() const noexcept
		{
			return m_region.m_width;
		}

		const int BatchSprite::get_height() const noexcept
		{
			return m_region.m_height;
		}

		const graphics::fRect& BatchSprite::get_region() const noexcept
		{
			return m_region;
		}

		const std::vector<glm::vec2>& BatchSprite::get_vertexs() const noexcept
		{
			return m_vertexs;
		}

		const std::string& BatchSprite::get_tex_id() const noexcept
		{
			return m_id;
		}

		nlohmann::json BatchSprite::serialize()
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

		void BatchSprite::deserialize(const nlohmann::json& json)
		{
			set_region(json.at("texture-atlas-id"));
			m_opacity = std::clamp(json.at("opacity").get<float>(), 0.0f, 1.0f);

			if (json.count("custom-width") > 0)
			{
				set_custom_width(json.at("custom-width"));
			}

			if (json.count("custom-height") > 0)
			{
				set_custom_height(json.at("custom-height"));
			}
		}
	} // namespace components
} // namespace galaxy