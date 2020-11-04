///
/// BatchedSprite.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include "BatchedSprite.hpp"

///
/// Core namespace.
///
namespace qs
{
	BatchedSprite::BatchedSprite()
	    : Transform {}, m_opacity {1.0f}, m_z_level {0}, m_offset {0}
	{
	}

	BatchedSprite::BatchedSprite(BatchedSprite&& bs)
	{
		this->m_opacity = bs.m_opacity;
		this->m_z_level = bs.m_z_level;
		this->m_offset  = bs.m_offset;
		this->m_region  = std::move(bs.m_region);
	}

	BatchedSprite& BatchedSprite::operator=(BatchedSprite&& bs)
	{
		if (this != &bs)
		{
			this->m_opacity = bs.m_opacity;
			this->m_z_level = bs.m_z_level;
			this->m_offset  = bs.m_offset;
			this->m_region  = std::move(bs.m_region);
		}

		return *this;
	}

	void BatchedSprite::create(const pr::Rect<float>& region, const unsigned int z_level) noexcept
	{
		m_region  = region;
		m_z_level = z_level;

		set_rotation_origin(m_region.m_width * 0.5f, m_region.m_height * 0.5f);

		m_opacity = 1.0f;
		m_dirty   = true;
	}

	void BatchedSprite::set_opacity(const float opacity) noexcept
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

	void BatchedSprite::set_z_level(const unsigned int level) noexcept
	{
		m_z_level = level;
		m_dirty   = true;
	}

	void BatchedSprite::update_x_region(const float x) noexcept
	{
		m_region.m_x = x;
		m_dirty      = true;
	}

	void BatchedSprite::update_y_region(const float y) noexcept
	{
		m_region.m_y = y;
		m_dirty      = true;
	}

	void BatchedSprite::update_w_region(const float w) noexcept
	{
		m_region.m_width = w;
		m_dirty          = true;
	}

	void BatchedSprite::update_h_region(const float h) noexcept
	{
		m_region.m_height = h;
		m_dirty           = true;
	}

	void BatchedSprite::update_region(float x, float y, const float w, const float h) noexcept
	{
		m_region.m_x      = x;
		m_region.m_y      = y;
		m_region.m_width  = w;
		m_region.m_height = h;

		m_dirty = true;
	}

	void BatchedSprite::update_region(const pr::Rect<float>& quad) noexcept
	{
		update_region(quad.m_x, quad.m_y, quad.m_width, quad.m_height);
	}

	const unsigned int BatchedSprite::z_level() const noexcept
	{
		return m_z_level;
	}

	const int BatchedSprite::get_width() const noexcept
	{
		return m_region.m_width;
	}

	const int BatchedSprite::get_height() const noexcept
	{
		return m_region.m_height;
	}
} // namespace qs