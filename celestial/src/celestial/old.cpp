///
/// UI.cpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#include "UI.hpp"

///
/// Core namespace.
///
namespace celestial
{
	UI::UI() noexcept
		:m_theme(nullptr), m_renderer(nullptr), m_isVisible(true), m_counter(0)
	{
	}

	UI::~UI() noexcept
	{
		destroy();
	}
	
	void UI::update(const double dt) noexcept
	{
		if (m_isVisible)
		{
			// Update all widgets.
			for (auto&& widget : m_widgets)
			{
				widget->update(dt);
			}
		}
	}

	void UI::render() noexcept
	{
		if (m_isVisible)
		{
			// Render all widgets.
			for (auto&& widget : m_widgets)
			{
				widget->render(m_renderer);
			}
		}
	}

	void UI::remove(const unsigned int id) noexcept
	{
		// Don't erase because that will mess up ordering.
		m_widgets[id].reset();
		m_widgets[id] = nullptr;

		m_free.push_back(id);
	}

	void UI::destroy() noexcept
	{
		for (auto&& widget : m_widgets)
		{
			widget.reset();
		}

		m_widgets.clear();
	}

	void UI::setRenderer(celestial::interface::Renderer* renderer) noexcept
	{
		m_renderer = renderer;
	}

	void UI::setTheme(celestial::UITheme* theme) noexcept
	{
		m_theme = theme;
	}

	void UI::isVisible(const bool isVisible) noexcept
	{
		m_isVisible = isVisible;
	}
}