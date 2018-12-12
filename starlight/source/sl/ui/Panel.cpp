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
	Panel::Panel(const sl::Rect<int>& bounds, const ALLEGRO_COLOR colour, const std::string& bgImage)
		:m_isVisible(true), m_bounds(bounds)
	{
		if (bgImage != "")
		{
			// If image is specified, load the image from the VFS. Allegro integrates with physfs naturally.
			m_background = al_load_bitmap(bgImage.c_str());
		}
		else
		{
			// What is done here is that the rendertarget is set to the bitmap and we clear and draw that colour to it.
			m_background = al_create_bitmap(m_bounds.m_width, m_bounds.m_height);

			al_set_target_bitmap(m_background);
			al_clear_to_color(colour);
			al_flip_display();

			al_set_target_backbuffer(Locator::window->getDisplay());
		}

		if (!m_background)
		{
			LOG_S(ERROR) << "Failed to load background: " << bgImage;
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

	void Panel::update()
	{
		// Update all widgets.
		for (auto& widget : m_widgets)
		{
			if (widget->isVisible())
			{
				widget->update();
			}
		}
	}

	void Panel::render()
	{
		// Render all widgets.
		for (auto& widget : m_widgets)
		{
			if (widget->isVisible())
			{
				widget->render();
			}
		}
	}

	void Panel::setVisibility(const bool isVisible)
	{
		m_isVisible = isVisible;
	}

	const bool Panel::isVisible() const
	{
		return m_isVisible;
	}

	void Panel::clear()
	{
		// Reset smart pointer then empty vector.
		for (auto& widget : m_widgets)
		{
			widget.reset();
		}

		m_widgets.clear();
	}
}