///
/// Image.cpp
/// starlight
///
/// Created by reworks on 97/12/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "Image.hpp"

namespace sl
{
	Image::Image(const int x, const int y, const std::string& texture)
		:Widget({ x, y, 0, 0 })
	{
		m_image = al_load_bitmap(texture.c_str());
		if (!m_image)
		{
			LOG_S(FATAL) << "Failed to load texture: " << texture << " Errno: " << al_get_errno();
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
}