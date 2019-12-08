///
/// Image.cpp
/// galaxy
///
/// Created by reworks on 97/12/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/libs/sol2/sol.hpp"

#include "Image.hpp"

namespace galaxy
{
	Image::Image(const int x, const int y, const std::string& texture, UITheme* theme)
		:Widget({ x, y, 0, 0 }, theme)
	{
		// Load image.
		m_image = m_theme->widgetTexture(texture);
		if (!m_image)
		{
			LOG_S(FATAL) << "Failed to create sub bitmap: " << texture << " Errno: " << al_get_errno();
		}
		else
		{
			// Set dimensions.
			m_bounds.m_width = al_get_bitmap_width(m_image);
			m_bounds.m_height = al_get_bitmap_height(m_image);
		}
	}

	Image::Image(const sol::table& table, UITheme* theme)
		:Widget({ 0, 0, 0, 0 }, theme)
	{
		// Get position data.
		m_bounds.m_x = table.get<int>("x");
		m_bounds.m_y = table.get<int>("y");

		// Load image.
		m_image = m_theme->widgetTexture(table.get<std::string>("texture"));
		if (!m_image)
		{
			LOG_S(FATAL) << "Failed to create sub bitmap: " << table.get<std::string>("texture") << " Errno: " << al_get_errno();
		}
		else
		{
			// Set dimensions.
			m_bounds.m_width = al_get_bitmap_width(m_image);
			m_bounds.m_height = al_get_bitmap_height(m_image);
		}
	}

	Image::~Image()
	{
		if (m_image)
		{
			al_destroy_bitmap(m_image);
		}
	}

	void Image::update(const double dt)
	{
	}

	void Image::render()
	{
		if (m_isVisible)
		{
			al_draw_bitmap(m_image, m_bounds.m_x, m_bounds.m_y, 0);
		}
	}

	void Image::setOffset(const int x, const int y)
	{
		m_bounds.m_x += x;
		m_bounds.m_y += y;
	}
}