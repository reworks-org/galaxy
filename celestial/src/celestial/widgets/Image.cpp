///
/// Image.cpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#include "Image.hpp"

///
/// Core namespace.
///
namespace celestial
{
	Image::Image(const int x, const int y, const std::string& texture, UITheme* theme)
		:Widget({ x, y, 0, 0 }, theme)
	{
		// Load image.
		m_image = m_theme->extractWidgetTexture(texture);
		
		// Set dimensions.
		m_bounds.m_width = m_theme->loader()->getTextureWidth(m_image.get());
		m_bounds.m_height = m_theme->loader()->getTextureHeight(m_image.get());
	}

	Image::~Image()
	{
		m_image.reset();
	}

	void Image::update(const double dt)
	{
	}

	void Image::render(celestial::Renderer* renderer)
	{
		if (m_isVisible)
		{
			renderer->drawTexture(m_image.get(), m_bounds.m_x, m_bounds.m_y);
		}
	}

	void Image::setOffset(const int x, const int y)
	{
		m_bounds.m_x += x;
		m_bounds.m_y += y;
	}
}