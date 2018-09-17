///
/// Panel.cpp
/// starlight
///
/// Created by reworks on 14/09/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "Panel.hpp"

namespace sl
{
	Panel::Panel(const sl::Rect<int>& bounds, const ALLEGRO_COLOR& colour) noexcept
		:m_isVisible(true), m_bounds(bounds), m_colour(colour)
	{
	}

	Panel::~Panel()
	{
		// Ensure widgets are cleaned up.
		m_widgets.clear();
	}

	void Panel::render()
	{
		if (m_isVisible)
		{
			for (auto& widget : m_widgets)
			{
				widget->render();
			}
		}
	}

	void Panel::isVisible(bool isVisible)
	{
		m_isVisible = isVisible;
	}
}