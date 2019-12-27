///
/// Panel.cpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#include "Panel.hpp"

///
/// Core namespace.
///
namespace celestial
{
	Panel::Panel(const protostar::Rect<int>& bounds, UITheme* theme)
		:m_counter(0), m_isVisible(true), m_bounds(bounds), m_background(nullptr), m_theme(theme)
	{
		m_background = m_theme->loader()->createRectangle(m_bounds.m_width, m_bounds.m_height, m_theme->colour());
	}

	Panel::Panel(const std::string& texture, const protostar::Rect<int>& bounds, UITheme* theme)
		:m_counter(0), m_isVisible(true), m_bounds(bounds), m_background(nullptr), m_theme(theme)
	{
		m_background = m_theme->extractWidgetTexture(texture);
	}

	Panel::~Panel()
	{
		m_background.reset();

		// Ensures widgets are cleaned up.
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

	void Panel::render(celestial::compat::Renderer* renderer)
	{
		if (m_isVisible)
		{
			renderer->drawTexture(m_background.get(), m_bounds.m_x, m_bounds.m_y);
			
			// Render all widgets.
			for (auto& pair : m_widgets)
			{
				pair.second->render(renderer);
			}
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