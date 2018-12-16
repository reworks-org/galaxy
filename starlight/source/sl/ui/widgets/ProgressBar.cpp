///
/// ProgressBar.cpp
/// starlight
///
/// Created by reworks on 16/12/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include <algorithm>
#include <allegro5/allegro_primitives.h>

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
			LOG_S(ERROR) << "Failed to create ProgressBar background bitmap.";
		}

		m_bar = al_create_bitmap(m_bounds.m_width, m_bounds.m_height);
		if (!m_bar)
		{
			LOG_S(ERROR) << "Failed to create ProgressBar bar bitmap.";
		}

		// Create bar bitmap.
		al_set_target_bitmap(m_bar);
		al_clear_to_color(foreCol);
		al_draw_filled_rectangle(m_bounds.m_x, m_bounds.m_y, m_bounds.m_x + m_bounds.m_width, m_bounds.m_y + m_bounds.m_height, foreCol);
		al_flip_display();

		// Create background bitmap.
		al_set_target_bitmap(m_background);
		al_clear_to_color(backCol);
		al_draw_filled_rectangle(m_bounds.m_x, m_bounds.m_y, m_bounds.m_x + m_bounds.m_width, m_bounds.m_y + m_bounds.m_height, backCol);
		al_flip_display();

		// Restore window.
		al_set_target_backbuffer(Locator::window->getDisplay());
		
		sl::Locator::dispatcher->sink<sl::MouseMovedEvent>().connect<ProgressBar, &ProgressBar::recieve>(this);
	}

	ProgressBar::ProgressBar(const sl::Rect<int>& bounds, const sl::Rect<int> barBounds, const std::string& image, const ALLEGRO_COLOR col)
		:Widget(bounds), m_barBounds(barBounds), m_progress(0.0f), m_background(nullptr), m_bar(nullptr)
	{
		m_bar = al_create_bitmap(m_bounds.m_width, m_bounds.m_height);
		if (!m_bar)
		{
			LOG_S(ERROR) << "Failed to create ProgressBar bar bitmap.";
		}

		// Create bar bitmap.
		al_set_target_bitmap(m_bar);
		al_clear_to_color(col);
		al_draw_filled_rectangle(m_bounds.m_x, m_bounds.m_y, m_bounds.m_x + m_bounds.m_width, m_bounds.m_y + m_bounds.m_height, col);
		al_flip_display();

		// Restore window.
		al_set_target_backbuffer(Locator::window->getDisplay());

		// Now the background / outline, etc..
		m_background = al_load_bitmap(image.c_str());
		if (!m_background)
		{
			LOG_S(ERROR) << "Failed to load ProgressBar background bitmap: " << image;
		}
	}

	ProgressBar::ProgressBar(const sl::Rect<int>& bounds, const sl::Rect<int> barBounds, const std::string& image, const std::string& barImage)
		:Widget(bounds), m_barBounds(barBounds), m_progress(0.0f), m_background(nullptr), m_bar(nullptr)
	{
		// The background / outline, etc..
		m_background = al_load_bitmap(image.c_str());
		if (!m_background)
		{
			LOG_S(ERROR) << "Failed to load ProgressBar background bitmap: " << image;
		}

		// Load the bar texture.
		m_bar = al_load_bitmap(barImage.c_str());
		if (!m_bar)
		{
			LOG_S(ERROR) << "Failed to load ProgressBar bar image bitmap: " << image;
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

	void ProgressBar::update()
	{
		m_barBounds.m_width = static_cast<int>(m_progress * m_bounds.m_width);
	}

	void ProgressBar::render()
	{
		al_draw_bitmap(m_background, m_bounds.m_x, m_bounds.m_y, 0);
		al_draw_bitmap_region(m_bar, 0, 0, m_barBounds.m_width, m_barBounds.m_height, m_barBounds.m_x, m_barBounds.m_y, 0);

		if (m_tooltip && m_drawTooltip)
		{
			m_tooltip->draw();
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
	
}