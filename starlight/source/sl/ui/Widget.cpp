///
/// Widget.cpp
/// starlight
///
/// Created by reworks on 15/09/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "Widget.hpp"

namespace sl
{
	BaseWidget::BaseWidget(const sl::Rect<int>& bounds) noexcept
		:m_isVisible(true), m_bounds(bounds)
	{
	}

	void BaseWidget::isVisible(bool isVisible)
	{
		m_isVisible = isVisible;
	}
}