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
	BatchedSprite::BatchedSprite() noexcept
		:Transform(), m_isDirty(false), m_opacity(1.0f), m_zLevel(0), m_offset(0)
	{
	}

	void BatchedSprite::create(const protostar::Rect<float>& region, const unsigned int zlevel) noexcept
	{
		m_region = region;
		m_zLevel = zlevel;

		setRotationOrigin(m_region.m_width * 0.5f, m_region.m_height * 0.5f);
		setOpacity(1.0f);

		m_isDirty = true;
	}

	void BatchedSprite::setOpacity(float opacity) noexcept
	{
		if (opacity > 1.0f)
		{
			opacity = 1.0f;
		}
		else if (opacity < 0.0f)
		{
			opacity = 0.0f;
		}

		m_opacity = opacity;
		m_isDirty = true;
	}

	void BatchedSprite::setZLevel(const unsigned int level) noexcept
	{
		m_zLevel = level;
		m_isDirty = true;
	}
	
	void BatchedSprite::setUpdatedRegion(float x, float y, float w, float h) noexcept
	{
		m_region.m_x = x;
		m_region.m_y = y;

		if (w >= 0.0f)
		{
			m_region.m_width = w;
		}

		if (h >= 0.0f)
		{
			m_region.m_height = h;
		}

		m_isDirty = true;
	}

	const unsigned int BatchedSprite::getZLevel() const noexcept
	{
		return m_zLevel;
	}
}