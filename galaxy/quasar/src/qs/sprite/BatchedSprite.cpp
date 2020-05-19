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
		:Transform(), m_opacity(1.0f), m_zLevel(0)
	{
	}

	void BatchedSprite::load(const protostar::Rect<float>& region, const unsigned int zlevel) noexcept
	{
		m_region = region;
		m_zLevel = zlevel;
	}

	void BatchedSprite::create() noexcept
	{
		setRotationOrigin(m_region.m_width * 0.5f, m_region.m_height * 0.5f);
		setOpacity(1.0f);
	}

	void BatchedSprite::create(const float x, const float y) noexcept
	{
		setPos(x, y);
		setRotationOrigin(m_region.m_width * 0.5f, m_region.m_height * 0.5f);
		setOpacity(1.0f);
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
	}

	const float BatchedSprite::getOpacity() const noexcept
	{
		return m_opacity;
	}
	const unsigned int BatchedSprite::getZLevel() const noexcept
	{
		return 0;
	}

	protostar::Rect<float>& BatchedSprite::getRegion() noexcept
	{
		return m_region;
	}
}