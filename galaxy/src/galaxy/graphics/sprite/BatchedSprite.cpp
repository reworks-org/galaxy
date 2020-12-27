///
/// BatchedSprite.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "BatchedSprite.hpp"

namespace galaxy
{
	namespace graphics
	{
		BatchedSprite::BatchedSprite()
		    : Transform {}, m_opacity {1.0f}, m_z_level {0}, m_offset {0}
		{
		}

		BatchedSprite::BatchedSprite(const nlohmann::json& json)
		{
			set_opacity(json.at("opacity"));
			set_pos(json.at("x"), json.at("y"));
			create({json.at("texture-x"), json.at("texture-y"), json.at("w"), json.at("h")}, json.at("z-level"));
		}

		BatchedSprite::~BatchedSprite()
		{
			m_offset = 0;
		}

		void BatchedSprite::create(const graphics::Rect<float>& region, const unsigned int z_level)
		{
			m_region  = region;
			m_z_level = z_level;

			set_rotation_origin(m_region.m_width * 0.5f, m_region.m_height * 0.5f);

			m_opacity = 1.0f;
			m_dirty   = true;
		}

		void BatchedSprite::set_opacity(const float opacity)
		{
			if (m_opacity > 1.0f)
			{
				m_opacity = 1.0f;
			}
			else if (m_opacity < 0.0f)
			{
				m_opacity = 0.0f;
			}
			else
			{
				m_opacity = opacity;
			}

			m_dirty = true;
		}

		void BatchedSprite::set_z_level(const unsigned int level)
		{
			m_z_level = level;
			m_dirty   = true;
		}

		void BatchedSprite::update_x_region(const float x)
		{
			m_region.m_x = x;
			m_dirty      = true;
		}

		void BatchedSprite::update_y_region(const float y)
		{
			m_region.m_y = y;
			m_dirty      = true;
		}

		void BatchedSprite::update_w_region(const float w)
		{
			m_region.m_width = w;
			m_dirty          = true;
		}

		void BatchedSprite::update_h_region(const float h)
		{
			m_region.m_height = h;
			m_dirty           = true;
		}

		void BatchedSprite::update_region(float x, float y, const float w, const float h)
		{
			m_region.m_x      = x;
			m_region.m_y      = y;
			m_region.m_width  = w;
			m_region.m_height = h;

			m_dirty = true;
		}

		void BatchedSprite::update_region(const graphics::Rect<float>& quad)
		{
			update_region(quad.m_x, quad.m_y, quad.m_width, quad.m_height);
		}

		const float BatchedSprite::opacity() const
		{
			return m_opacity;
		}

		const unsigned int BatchedSprite::z_level() const
		{
			return m_z_level;
		}

		const int BatchedSprite::get_width() const
		{
			return m_region.m_width;
		}

		const int BatchedSprite::get_height() const
		{
			return m_region.m_height;
		}

		const graphics::Rect<float>& BatchedSprite::get_region() const
		{
			return m_region;
		}
	} // namespace graphics
} // namespace galaxy