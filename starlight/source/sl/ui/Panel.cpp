///
/// Panel.cpp
/// starlight
///
/// Created by reworks on 14/09/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/graphics/Window.hpp"
#include "sl/libs/loguru/loguru.hpp"
#include "sl/core/ServiceLocator.hpp"

#include "Panel.hpp"

namespace sl
{
	Panel::Panel(const sl::Rect<int>& bounds, const ALLEGRO_COLOR colour)
		:m_counter(0), m_isVisible(true), m_bounds(bounds), m_background(nullptr)
	{
		// What is done here is that the rendertarget is set to the bitmap and we clear and draw that colour to it.
		m_background = al_create_bitmap(m_bounds.m_width, m_bounds.m_height);
		if (!m_background)
		{
			LOG_S(ERROR) << "Failed to create background for panel! Errno: " << al_get_errno();
		}

		// This just clears the bitmap to the colour we want. Simple way to create rectangle.
		al_set_target_bitmap(m_background);
		al_clear_to_color(colour);
		al_flip_display();

		// Then restore window as display render target.
		al_set_target_backbuffer(Locator::window->getDisplay());
	}

	Panel::Panel(const sl::Rect<int>& bounds, UITheme* theme, const std::string& texture)
		:m_counter(0), m_isVisible(true), m_bounds(bounds), m_background(nullptr)
	{
		// Load image and check for errors.
		m_background = theme->widgetTexture(texture);
		if (!m_background)
		{
			LOG_S(ERROR) << "Failed to load background: " << texture << ". From theme widget. al_errno: " << al_get_errno();
		}
	}

	Panel::~Panel()
	{
		// Destroy background.
		if (m_background)
		{
			al_destroy_bitmap(m_background);
		}

		// Ensure widgets are cleaned up.
		clear();
	}

	void Panel::remove(unsigned int id)
	{
		// Make sure widget actually exists before trying to remove it.
		auto found = m_widgets.find(id);
		if (found != m_widgets.end())
		{
			m_widgets.erase(id);
		}
		else
		{
			LOG_S(WARNING) << "Tried to remove a nonexistant widget of id: " << id;
		}
	}

	void Panel::update(const double dt)
	{
		if (m_isVisible)
		{
			// Update all widgets.
			for (auto& pair : m_widgets)
			{
				pair.second->update(dt);
			}
		}
	}

	void Panel::render()
	{
		if (m_isVisible)
		{
			// Draw panel.
			al_draw_bitmap(m_background, m_bounds.m_x, m_bounds.m_y, 0);

			al_hold_bitmap_drawing(true);

			// Render all widgets.
			for (auto& pair : m_widgets)
			{
				pair.second->render();
			}

			al_hold_bitmap_drawing(false);
		}
	}

	void Panel::setVisibility(const bool isVisible)
	{
		m_isVisible = isVisible;

		for (auto& pair : m_widgets)
		{
			pair.second->setVisibility(isVisible);
		}
	}

	void Panel::clear()
	{
		// Reset smart pointer then empty vector.
		for (auto& pair : m_widgets)
		{
			pair.second.reset();
		}

		m_widgets.clear();
	}
}