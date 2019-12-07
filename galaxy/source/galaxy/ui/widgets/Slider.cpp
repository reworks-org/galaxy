///
/// Slider.cpp
/// starlight
///
/// Created by reworks on 16/12/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/graphics/Window.hpp"
#include "sl/core/ServiceLocator.hpp"
#include "sl/libs/entt/signal/dispatcher.hpp"

#include "Slider.hpp"

namespace sl
{
	Slider::Slider(const sl::Rect<int>& bounds, const int mw, const int mh, const ALLEGRO_COLOR slider, const ALLEGRO_COLOR marker)
		:Widget(bounds, nullptr), m_value(0.0f), m_markerX(0.0f), m_slider(nullptr), m_marker(nullptr)
	{
		// Create textures
		m_slider = al_create_bitmap(m_bounds.m_width, m_bounds.m_height);
		if (!m_slider)
		{
			LOG_S(FATAL) << "Failed to create Slider bitmap.  Errno: " << al_get_errno();
		}

		m_marker = al_create_bitmap(mw, mh);
		if (!m_marker)
		{
			LOG_S(FATAL) << "Failed to create Slider marker bitmap. Errno: " << al_get_errno();
		}

		// Create slider bitmap.
		al_set_target_bitmap(m_slider);
		al_clear_to_color(slider);
		al_flip_display();

		// Create marker bitmap.
		al_set_target_bitmap(m_marker);
		al_clear_to_color(marker);
		al_flip_display();

		// Restore window.
		al_set_target_backbuffer(Locator::window->getDisplay());

		// Register events.
		sl::Locator::dispatcher->sink<sl::MouseMovedEvent>().connect<Slider, &Slider::receiveMove>(this);
		sl::Locator::dispatcher->sink<sl::MousePressedEvent>().connect<Slider, &Slider::receivePress>(this);
	}

	Slider::Slider(const int x, const int y, const std::string& slider, const std::string& marker, UITheme* theme)
		:Widget({ x, y, 0, 0 }, theme), m_value(0.0f), m_markerX(0.0f), m_slider(nullptr), m_marker(nullptr)
	{
		// Load slider texture and check for errors.
		m_slider = m_theme->widgetTexture(slider);
		if (!m_slider)
		{
			LOG_S(FATAL) << "Failed to create sub bitmap Slider: " << slider << " Errno: " << al_get_errno();
		}
		else
		{
			// Set dimensions.
			m_bounds.m_width = al_get_bitmap_width(m_slider);
			m_bounds.m_height = al_get_bitmap_height(m_slider);
		}

		// Load marker texture and check for errors.
		m_marker = m_theme->widgetTexture(marker);
		if (!m_marker)
		{
			LOG_S(FATAL) << "Failed to crate sub bitmap Slider marker: " << marker << " Errno: " << al_get_errno();
		}
	}

	Slider::Slider(const sol::table& table, UITheme* theme)
		:Widget({ 0, 0, 0, 0 }, theme), m_value(0.0f), m_markerX(0.0f), m_slider(nullptr), m_marker(nullptr)
	{
		// Get position data.
		m_bounds.m_x = table.get<int>("x");
		m_bounds.m_y = table.get<int>("y");

		// Load slider texture and check for errors.
		m_slider = m_theme->widgetTexture(table.get<std::string>("slider"));
		if (!m_slider)
		{
			LOG_S(FATAL) << "Failed to create sub bitmap Slider: " << table.get<std::string>("slider") << " Errno: " << al_get_errno();
		}
		else
		{
			// Set dimensions.
			m_bounds.m_width = al_get_bitmap_width(m_slider);
			m_bounds.m_height = al_get_bitmap_height(m_slider);
		}

		// Load marker texture and check for errors.
		m_marker = m_theme->widgetTexture(table.get<std::string>("marker"));
		if (!m_marker)
		{
			LOG_S(FATAL) << "Failed to crate sub bitmap Slider marker: " << table.get<std::string>("marker") << " Errno: " << al_get_errno();
		}
	}

	Slider::~Slider() noexcept
	{
		if (m_slider)
		{
			al_destroy_bitmap(m_slider);
		}

		if (m_marker)
		{
			al_destroy_bitmap(m_marker);
		}
	}

	void Slider::receiveMove(const sl::MouseMovedEvent& e)
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

	void Slider::receivePress(const sl::MousePressedEvent& e)
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
			m_markerX = (m_bounds.m_x + (m_bounds.m_width * m_value)) - (static_cast<float>(al_get_bitmap_width(m_marker)) / 2.0f);
		}
	}

	void Slider::render()
	{
		if (m_isVisible)
		{
			al_draw_bitmap(m_slider, m_bounds.m_x, m_bounds.m_y, 0);

			// we center the marker so it is exactly half way on the texture.
			al_draw_bitmap(m_marker, m_markerX, // x
				static_cast<float>(m_bounds.m_y) - (static_cast<float>(al_get_bitmap_height(m_marker)) / 2.0f) + (static_cast<float>(al_get_bitmap_height(m_slider)) / 2.0f), // y
				0);

			if (m_tooltip && m_drawTooltip)
			{
				m_tooltip->draw();
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