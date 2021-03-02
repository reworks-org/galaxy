///
/// Sprite2D.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <algorithm>

#include <nlohmann/json.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/res/TextureAtlas.hpp"

#include "Sprite2D.hpp"

namespace galaxy
{
	namespace components
	{
		Sprite2D::Sprite2D() noexcept
		    : Serializable {this}, m_opacity {1.0f}, m_region {0.0f, 0.0f, 0.0f, 0.0f}, m_offset {0}, m_z_level {0}, m_custom_wh {0.0f, 0.0f}
		{
		}

		Sprite2D::Sprite2D(const nlohmann::json& json)
		    : Serializable {this}, m_opacity {1.0f}, m_region {0.0f, 0.0f, 0.0f, 0.0f}, m_offset {0}, m_z_level {0}, m_custom_wh {0.0f, 0.0f}
		{
			deserialize(json);
		}

		Sprite2D::Sprite2D(Sprite2D&& bs) noexcept
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

		Sprite2D& Sprite2D::operator=(Sprite2D&& bs) noexcept
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

		Sprite2D::~Sprite2D() noexcept
		{
			m_offset = 0;
		}

		void Sprite2D::create(const graphics::fRect& region, float opacity)
		{
			m_region  = region;
			m_opacity = std::clamp(opacity, 0.0f, 1.0f);
		}

		void Sprite2D::create(std::string_view texture_atlas_id, float opacity)
		{
			m_region  = SL_HANDLE.atlas()->get_region(texture_atlas_id);
			m_opacity = std::clamp(opacity, 0.0f, 1.0f);
		}

		void Sprite2D::set_region(std::string_view region)
		{
			m_id     = region;
			m_region = SL_HANDLE.atlas()->get_region(m_id);

			m_custom_wh = {0.0f, 0.0f};
		}

		void Sprite2D::set_opacity(const float opacity) noexcept
		{
			m_opacity = std::clamp(opacity, 0.0f, 1.0f);
		}

		void Sprite2D::set_custom_width(const float width) noexcept
		{
			m_custom_wh.x    = width;
			m_region.m_width = width;
		}

		void Sprite2D::set_custom_height(const float height) noexcept
		{
			m_custom_wh.y     = height;
			m_region.m_height = height;
		}

		const float Sprite2D::get_opacity() const noexcept
		{
			return m_opacity;
		}

		const int Sprite2D::get_width() const noexcept
		{
			return m_region.m_width;
		}

		const int Sprite2D::get_height() const noexcept
		{
			return m_region.m_height;
		}

		const graphics::fRect& Sprite2D::get_region() const noexcept
		{
			return m_region;
		}

		const std::vector<glm::vec2>& Sprite2D::get_vertexs() const noexcept
		{
			return m_vertexs;
		}

		nlohmann::json Sprite2D::serialize()
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

		void Sprite2D::deserialize(const nlohmann::json& json)
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