///
/// Slider.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <algorithm>

#include "Slider.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	Slider::Slider(const pr::Rect<int>& bounds, const int mw, const int mh, const pr::Colour& slider, const pr::Colour& marker, galaxy::ResourceLoader* loader)
		:Widget(bounds, nullptr), m_value(0.0f), m_markerX(0.0f), m_markerW(mw), m_slider(nullptr), m_marker(nullptr)
	{
		// Create textures
		m_slider = loader->createRectangle(m_bounds.m_width, m_bounds.m_height, slider);
		m_marker = loader->createRectangle(mw, mh, marker);
	}

	Slider::Slider(const int x, const int y, const std::string& slider, const std::string& marker, UITheme* theme)
		:Widget({ x, y, 0, 0 }, theme), m_value(0.0f), m_markerX(0.0f), m_slider(nullptr), m_marker(nullptr)
	{
		// Load slider texture and check for errors.
		m_slider = m_theme->extractWidgetTexture(slider);
		
		// Set dimensions.
		m_bounds.m_width = m_theme->loader()->getTextureWidth(m_slider.get());
		m_bounds.m_height = m_theme->loader()->getTextureHeight(m_slider.get());

		// Load marker texture and check for errors.
		m_marker = m_theme->extractWidgetTexture(marker);
		m_markerW = m_theme->loader()->getTextureWidth(m_marker.get());
		m_markerH = m_theme->loader()->getTextureHeight(m_marker.get());
	}

	Slider::~Slider() noexcept
	{
		m_slider.reset();
		m_marker.reset();
	}

	void Slider::receiveMove(const pr::MouseMovedEvent& e)
	{
		if (m_isVisible)
		{
			if (contains(e.m_x, e.m_y))
			{
				m_drawTooltip = true;

				if (e.m_pressure == 1.0f)
				{
					m_drawTooltip = false;
					int cursorPosOnSlider = e.m_x - m_bounds.m_x;
					m_value = std::clamp(static_cast<float>(cursorPosOnSlider) / static_cast<float>(m_bounds.m_width), 0.0f, 1.0f);
				}
			}
			else
			{
				m_drawTooltip = false;
			}
		}
	}

	void Slider::receivePress(const pr::MousePressedEvent& e)
	{
		if (m_isVisible)
		{
			if (contains(e.m_x, e.m_y) && (e.m_button == 1))
			{
				// We take away the button top left pos, because then we are left over with the position of the cursor on the widget.
				// Then make sure the resulting value is within the bar width.
				m_drawTooltip = false;
				int cursorPosOnSlider = e.m_x - m_bounds.m_x;
				m_value = std::clamp(static_cast<float>(cursorPosOnSlider) / static_cast<float>(m_bounds.m_width), 0.0f, 1.0f);
			}
		}
	}

	void Slider::update(const double dt)
	{
		if (m_isVisible)
		{
			// Draw marker centered on value by taking away half the width of the marker.
			m_markerX = (m_bounds.m_x + (m_bounds.m_width * m_value)) - (m_markerW / 2.0f);
		}
	}

	void Slider::render(galaxy::Renderer* renderer)
	{
		if (m_isVisible)
		{
			renderer->drawTexture(m_slider.get(), m_bounds.m_x, m_bounds.m_y);

			// we center the marker so it is exactly half way on the texture.
			renderer->drawTexture(m_marker.get(), m_markerX, // x
				static_cast<float>(m_bounds.m_y) - (m_markerH / 2.0f) + (static_cast<float>(m_bounds.m_height) / 2.0f) // y
			);

			if (m_tooltip && m_drawTooltip)
			{
				m_tooltip->draw(renderer);
			}
		}
	}

	const float Slider::getValue() const
	{
		return m_value;
	}

	const float Slider::getPercentage() const
	{
		return m_value * 100.0f;
	}

	void Slider::setOffset(const int x, const int y)
	{
		m_bounds.m_x += x;
		m_bounds.m_y += y;
	}
}