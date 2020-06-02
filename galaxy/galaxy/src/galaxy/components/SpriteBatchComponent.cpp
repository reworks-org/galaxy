///
/// SpriteBatchComponent.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "SpriteBatchComponent.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	SpriteBatchComponent::SpriteBatchComponent() noexcept
		:m_isDirty(false), m_opacity(1.0f), m_zLevel(0), m_offset(0)
	{
	}

	SpriteBatchComponent::SpriteBatchComponent(const nlohmann::json& json) noexcept
	{
		m_opacity = json.at("opacity");
		m_zLevel = json.at("z-level");

		create({ json.at("x") , json.at("y") , json.at("width") , json.at("height") }, m_zLevel);
	}

	void SpriteBatchComponent::create(const protostar::Rect<float>& region, const unsigned int zlevel) noexcept
	{
		m_region = region;
		m_zLevel = zlevel;

		setOpacity(1.0f);

		m_isDirty = true;
	}

	void SpriteBatchComponent::setOpacity(float opacity) noexcept
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

	void SpriteBatchComponent::setZLevel(const unsigned int level) noexcept
	{
		m_zLevel = level;
		m_isDirty = true;
	}

	void SpriteBatchComponent::setUpdatedRegion(float x, float y, float w, float h) noexcept
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

	const unsigned int SpriteBatchComponent::getZLevel() const noexcept
	{
		return m_zLevel;
	}
}