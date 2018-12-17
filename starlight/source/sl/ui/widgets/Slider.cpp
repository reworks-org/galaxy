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
	Slider::Slider(const sl::Rect<int>& bounds, const int markerW, const int markerH, const ALLEGRO_COLOR slider, const ALLEGRO_COLOR marker)
		:Widget(bounds), m_value(0.0f), m_markerX(0.0f), m_slider(nullptr), m_marker(nullptr)
	{
		// Create textures
		m_slider = al_create_bitmap(m_bounds.m_width, m_bounds.m_height);
		if (!m_slider)
		{
			LOG_S(ERROR) << "Failed to create Slider bitmap.  Errno: " << al_get_errno();
		}

		m_marker = al_create_bitmap(markerW, markerH);
		if (!m_marker)
		{
			LOG_S(ERROR) << "Failed to create Slider marker bitmap. Errno: " << al_get_errno();
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
		//sl::Locator::dispatcher->sink<sl::MouseMovedEvent>().connect<Slider, &Slider::receiveMove>(this);
		sl::Locator::dispatcher->sink<sl::MousePressedEvent>().connect<Slider, &Slider::receivePress>(this);
	}

	Slider::Slider(const sl::Rect<int>& bounds, const std::string& slider, const std::string& marker)
		:Widget(bounds), m_value(0.0f), m_markerX(0.0f), m_slider(nullptr), m_marker(nullptr)
	{
		// Load slider texture and check for errors.
		m_slider = al_load_bitmap(slider.c_str());
		if (!m_slider)
		{
			LOG_S(ERROR) << "Failed to load Slider bitmap: " << slider << " Errno: " << al_get_errno();
		}

		// Load marker texture and check for errors.
		m_marker = al_load_bitmap(marker.c_str());
		if (!m_marker)
		{
			LOG_S(ERROR) << "Failed to load Slider marker bitmap: " << marker << " Errno: " << al_get_errno();
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

	void Slider::receivePress(const sl::MousePressedEvent& e)
	{
		if (m_isVisible)
		{
			// If the mouse cursor is greater than the x axis but less than the total width of the button, and
			// Less than the height of the cursor, but greather than the y of the cursor take its height.

			int topleft = m_bounds.m_x + m_offsetX;
			int topright = topleft + m_bounds.m_width;
			int top = m_bounds.m_y + m_offsetY;
			int bottom = top + m_bounds.m_height;

			if ( ((e.m_x >= topleft) && (e.m_x <= topright) && (e.m_y >= top) && (e.m_y <= bottom)) && (e.m_button == 1) )
			{
				// We take away the button top left pos, because then we are left over with the position of the cursor on the widget.
				// Then make sure the resulting value is within the bar width.
				int cursorPosOnSlider = std::clamp(e.m_x - topleft, topleft, topright);
				m_value = std::clamp(static_cast<float>(cursorPosOnSlider) / static_cast<float>(topright), 0.0f, 1.0f);
			}

			m_drawTooltip = false;
		}
	}

	void Slider::update()
	{
		// Draw marker centered on value.
		m_markerX = m_value * (m_bounds.m_x + m_offsetX + m_bounds.m_width);
	}

	void Slider::render()
	{
		if (m_isVisible)
		{
			al_draw_bitmap(m_slider, m_bounds.m_x + m_offsetX, m_bounds.m_y + m_offsetY, 0);
			al_draw_bitmap(m_marker, static_cast<int>(m_markerX), m_bounds.m_y + m_offsetY, 0);

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
}