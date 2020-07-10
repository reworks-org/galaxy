///
/// Label.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <algorithm>

#include "ProgressBar.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	ProgressBar::ProgressBar(const protostar::Rect<int>& containerBarCoords, const std::string& container, const std::string& bar, UITheme* theme)
		:Widget({ containerBarCoords.m_x, containerBarCoords.m_y, 0, 0 }, theme), m_barBounds({ containerBarCoords.m_width, containerBarCoords.m_height, 0, 0 }), m_progress(0.0f), m_container(nullptr), m_bar(nullptr)
	{
		// The background / outline, etc..
		m_container = m_theme->extractWidgetTexture(container);
		
		// Set dimensions.
		m_bounds.m_width = m_theme->loader()->getTextureWidth(m_container.get());
		m_bounds.m_height = m_theme->loader()->getTextureHeight(m_container.get());

		// Load the bar texture.
		m_bar = m_theme->extractWidgetTexture(bar);
		
		// Set dimensions.
		m_barBounds.m_width = m_theme->loader()->getTextureWidth(m_bar.get());
		m_barBounds.m_height = m_theme->loader()->getTextureHeight(m_bar.get());
	}

	ProgressBar::ProgressBar(const protostar::Rect<int>& bounds, const protostar::Colour& container, const protostar::Colour& bar, galaxy::ResourceLoader* loader)
		:Widget(bounds, nullptr), m_barBounds(bounds), m_progress(0.0f), m_container(nullptr), m_bar(nullptr)
	{
		// Create textures
		m_container = loader->createRectangle(m_bounds.m_width, m_bounds.m_height, container);
		m_bar = loader->createRectangle(m_bounds.m_width, m_bounds.m_height, container);
	}

	ProgressBar::~ProgressBar() noexcept
	{
		m_container.reset();
		m_bar.reset();
	}

	void ProgressBar::recieve(const protostar::MouseMovedEvent& e)
	{
		if (m_isVisible)
		{
			if (contains(e.m_x, e.m_y))
			{
				m_drawTooltip = true;
			}
			else
			{
				m_drawTooltip = false;
			}
		}
	}

	void ProgressBar::update(const double dt)
	{
		if (m_isVisible)
		{
			// Recalculate width of the bar by the current "progress" to indicate how much has been done.
			m_barBounds.m_width = static_cast<int>(m_progress * m_bounds.m_width);
		}
	}

	void ProgressBar::render(galaxy::Renderer* renderer)
	{
		if (m_isVisible)
		{
			renderer->drawTexture(m_container.get(), m_bounds.m_x, m_bounds.m_y);
			renderer->drawTextureRegion(m_bar.get(), m_barBounds.m_x, m_barBounds.m_y, { 0, 0, m_barBounds.m_width, m_barBounds.m_height });

			if (m_tooltip && m_drawTooltip)
			{
				m_tooltip->draw(renderer);
			}
		}
	}

	void ProgressBar::setProgress(const int progress)
	{
		// Ensure value is correctly clamped.
		m_progress = std::clamp(static_cast<float>(progress), 0.0f, 1.0f);
	}

	const float ProgressBar::getProgress() const
	{
		return m_progress;
	}

	const float ProgressBar::getPercentage() const
	{
		return m_progress * 100.0f;
	}

	void ProgressBar::setOffset(const int x, const int y)
	{
		m_bounds.m_x += x;
		m_bounds.m_y += y;
	}
}