///
/// ProgressBar.cpp
/// starlight
///
/// Created by reworks on 16/12/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include <algorithm>

#include "sl/libs/sol2/sol.hpp"
#include "sl/graphics/Window.hpp"
#include "sl/core/ServiceLocator.hpp"
#include "sl/libs/entt/signal/dispatcher.hpp"

#include "ProgressBar.hpp"

namespace sl
{
	ProgressBar::ProgressBar(const sl::Rect<int>& bounds, const ALLEGRO_COLOR container, const ALLEGRO_COLOR bar)
		:Widget(bounds, nullptr), m_barBounds(bounds), m_progress(0.0f), m_container(nullptr), m_bar(nullptr)
	{
		// Create textures
		m_container = al_create_bitmap(m_bounds.m_width, m_bounds.m_height);
		if (!m_container)
		{
			LOG_S(FATAL) << "Failed to create ProgressBar background bitmap.  Errno: " << al_get_errno();
		}

		m_bar = al_create_bitmap(m_bounds.m_width, m_bounds.m_height);
		if (!m_bar)
		{
			LOG_S(FATAL) << "Failed to create ProgressBar bar bitmap. Errno: " << al_get_errno();
		}

		// Create container bitmap.
		al_set_target_bitmap(m_container);
		al_clear_to_color(container);
		al_flip_display();

		// Create bar bitmap.
		al_set_target_bitmap(m_bar);
		al_clear_to_color(bar);
		al_flip_display();

		// Restore window.
		al_set_target_backbuffer(Locator::window->getDisplay());
		
		// Register events.
		sl::Locator::dispatcher->sink<sl::MouseMovedEvent>().connect<ProgressBar, &ProgressBar::recieve>(this);
	}

	ProgressBar::ProgressBar(const sl::Vector4<int>& vec4, const std::string& container, const std::string& bar, UITheme* theme)
		:Widget({ vec4.m_w, vec4.m_x, 0, 0 }, theme), m_barBounds({ vec4.m_y, vec4.m_z, 0, 0 }), m_progress(0.0f), m_container(nullptr), m_bar(nullptr)
	{
		// The background / outline, etc..
		m_container = m_theme->widgetTexture(container);
		if (!m_container)
		{
			LOG_S(FATAL) << "Failed to create subbitmap ProgressBar container: " << container << " Errno: " << al_get_errno();
		}
		else
		{
			// Set dimensions.
			m_bounds.m_width = al_get_bitmap_width(m_container);
			m_bounds.m_height = al_get_bitmap_height(m_container);
		}

		// Load the bar texture.
		m_bar = m_theme->widgetTexture(bar);
		if (!m_bar)
		{
			LOG_S(FATAL) << "Failed to create subbitmap ProgressBar bar: " << bar << " Errno: " << al_get_errno();
		}
		else
		{
			// Set dimensions.
			m_barBounds.m_width = al_get_bitmap_width(m_bar);
			m_barBounds.m_height = al_get_bitmap_height(m_bar);
		}

		// Register events.
		sl::Locator::dispatcher->sink<sl::MouseMovedEvent>().connect<ProgressBar, &ProgressBar::recieve>(this);
	}

	ProgressBar::ProgressBar(const sol::table& table, UITheme* theme)
		:Widget({ 0, 0, 0, 0 }, theme), m_barBounds({ 0, 0, 0, 0 }), m_progress(0.0f), m_container(nullptr), m_bar(nullptr)
	{
		// Get position data.
		m_bounds.m_x = table.get<int>("x");
		m_bounds.m_y = table.get<int>("y");

		m_barBounds.m_x = table.get<int>("barX");
		m_barBounds.m_y = table.get<int>("barY");

		// The background / outline, etc..
		m_container = m_theme->widgetTexture(table.get<std::string>("container"));
		if (!m_container)
		{
			LOG_S(FATAL) << "Failed to create subbitmap ProgressBar container: " << table.get<std::string>("container") << " Errno: " << al_get_errno();
		}
		else
		{
			// Set dimensions.
			m_bounds.m_width = al_get_bitmap_width(m_container);
			m_bounds.m_height = al_get_bitmap_height(m_container);
		}

		// Load the bar texture.
		m_bar = m_theme->widgetTexture(table.get<std::string>("bar"));
		if (!m_bar)
		{
			LOG_S(FATAL) << "Failed to create subbitmap ProgressBar bar: " << table.get<std::string>("bar") << " Errno: " << al_get_errno();
		}
		else
		{
			// Set dimensions.
			m_barBounds.m_width = al_get_bitmap_width(m_bar);
			m_barBounds.m_height = al_get_bitmap_height(m_bar);
		}

		// Register events.
		sl::Locator::dispatcher->sink<sl::MouseMovedEvent>().connect<ProgressBar, &ProgressBar::recieve>(this);
	}

	ProgressBar::~ProgressBar() noexcept
	{
		if (m_container)
		{
			al_destroy_bitmap(m_container);
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

	void ProgressBar::render()
	{
		if (m_isVisible)
		{
			al_draw_bitmap(m_container, m_bounds.m_x, m_bounds.m_y, 0);
			al_draw_bitmap_region(m_bar, 0, 0, m_barBounds.m_width, m_barBounds.m_height, m_barBounds.m_x, m_barBounds.m_y, 0);

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