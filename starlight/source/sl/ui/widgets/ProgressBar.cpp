///
/// ProgressBar.cpp
/// starlight
///
/// Created by reworks on 16/12/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include <algorithm>

#include "sl/graphics/Window.hpp"
#include "sl/core/ServiceLocator.hpp"
#include "sl/libs/entt/signal/dispatcher.hpp"

#include "ProgressBar.hpp"

namespace sl
{
	ProgressBar::ProgressBar(const sl::Rect<int>& bounds, const ALLEGRO_COLOR foreCol, const ALLEGRO_COLOR backCol)
		:Widget(bounds), m_barBounds(bounds), m_progress(0.0f), m_background(nullptr), m_bar(nullptr)
	{
		// Create textures
		m_background = al_create_bitmap(m_bounds.m_width, m_bounds.m_height);
		if (!m_background)
		{
			LOG_S(FATAL) << "Failed to create ProgressBar background bitmap.  Errno: " << al_get_errno();
		}

		m_bar = al_create_bitmap(m_bounds.m_width, m_bounds.m_height);
		if (!m_bar)
		{
			LOG_S(FATAL) << "Failed to create ProgressBar bar bitmap. Errno: " << al_get_errno();
		}

		// Create bar bitmap.
		al_set_target_bitmap(m_bar);
		al_clear_to_color(foreCol);
		al_flip_display();

		// Create background bitmap.
		al_set_target_bitmap(m_background);
		al_clear_to_color(backCol);
		al_flip_display();

		// Restore window.
		al_set_target_backbuffer(Locator::window->getDisplay());
		
		// Register events.
		sl::Locator::dispatcher->sink<sl::MouseMovedEvent>().connect<ProgressBar, &ProgressBar::recieve>(this);
	}

	ProgressBar::ProgressBar(const int x, const int y, const sl::Rect<int> barBounds, const std::string& texture, const ALLEGRO_COLOR col)
		:Widget({ x, y, 0, 0 }), m_barBounds(barBounds), m_progress(0.0f), m_background(nullptr), m_bar(nullptr)
	{
		m_bar = al_create_bitmap(m_bounds.m_width, m_bounds.m_height);
		if (!m_bar)
		{
			LOG_S(FATAL) << "Failed to create ProgressBar bar bitmap. Errno: " << al_get_errno();
		}
		else
		{
			// Set dimensions.
			m_bounds.m_width = al_get_bitmap_width(m_bar);
			m_bounds.m_height = al_get_bitmap_height(m_bar);
		}

		// Create bar bitmap.
		al_set_target_bitmap(m_bar);
		al_clear_to_color(col);
		al_flip_display();

		// Restore window.
		al_set_target_backbuffer(Locator::window->getDisplay());

		// Now the background / outline, etc..
		m_background = al_load_bitmap(texture.c_str());
		if (!m_background)
		{
			LOG_S(FATAL) << "Failed to load ProgressBar background bitmap: " << texture << " Errno: " << al_get_errno();
		}
	}

	ProgressBar::ProgressBar(const int x, const int y, const int barX, const int barY, const std::string& texture, const std::string& barTexture)
		:Widget({ x, y, 0, 0 }), m_barBounds({ barX, barY, 0, 0 }), m_progress(0.0f), m_background(nullptr), m_bar(nullptr)
	{
		// The background / outline, etc..
		m_background = al_load_bitmap(texture.c_str());
		if (!m_background)
		{
			LOG_S(FATAL) << "Failed to load ProgressBar background bitmap: " << texture << " Errno: " << al_get_errno();
		}
		else
		{
			// Set dimensions.
			m_bounds.m_width = al_get_bitmap_width(m_background);
			m_bounds.m_height = al_get_bitmap_height(m_background);
		}

		// Load the bar texture.
		m_bar = al_load_bitmap(barTexture.c_str());
		if (!m_bar)
		{
			LOG_S(FATAL) << "Failed to load ProgressBar bar image bitmap: " << barTexture << " Errno: " << al_get_errno();
		}
		else
		{
			// Set dimensions.
			m_barBounds.m_width = al_get_bitmap_width(m_bar);
			m_barBounds.m_height = al_get_bitmap_height(m_bar);
		}
	}

	ProgressBar::~ProgressBar() noexcept
	{
		if (m_background)
		{
			al_destroy_bitmap(m_background);
		}

		if (m_bar)
		{
			al_destroy_bitmap(m_bar);
		}
	}

	void ProgressBar::recieve(const sl::MouseMovedEvent& e)
	{
		if (m_isVisible)
		{
			// If the mouse cursor is greater than the x axis but less than the total width of the button, and
			// Less than the height of the cursor, but greather than the y of the cursor take its height.

			int topleft = m_bounds.m_x + m_offsetX;
			int topright = topleft + m_bounds.m_width;
			int top = m_bounds.m_y + m_offsetY;
			int bottom = top + m_bounds.m_height;

			if ((e.m_x >= topleft) && (e.m_x <= topright) && (e.m_y >= top) && (e.m_y <= bottom))
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
			m_barBounds.m_width = static_cast<int>(m_progress * m_bounds.m_width);
		}
	}

	void ProgressBar::render()
	{
		if (m_isVisible)
		{
			al_draw_bitmap(m_background, m_bounds.m_x + m_offsetX, m_bounds.m_y + m_offsetY, 0);
			al_draw_bitmap_region(m_bar, 0, 0, m_barBounds.m_width, m_barBounds.m_height, m_barBounds.m_x + m_offsetX, m_barBounds.m_y + m_offsetY, 0);

			if (m_tooltip && m_drawTooltip)
			{
				m_tooltip->draw();
			}
		}
	}

	void ProgressBar::setProgress(const float progress)
	{
		// Ensure value is correctly clamped.
		m_progress = std::clamp(progress, 0.0f, 1.0f);
	}

	const float ProgressBar::getProgress() const
	{
		return m_progress;
	}

	const float ProgressBar::getPercentage() const
	{
		return m_progress * 100.0f;
	}
}