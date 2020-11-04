///
/// Widget.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Widget.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	const unsigned int Widget::id() const
	{
		return m_id;
	}

	Widget::Widget()
	    : m_id {0}, m_theme {nullptr}, m_tooltip {nullptr}, m_bounds {0.0f, 0.0f, 0.0f, 0.0f}
	{
	}

	Widget::Widget(Widget&& w)
	{
		this->m_id      = w.m_id;
		this->m_theme   = w.m_theme;
		this->m_tooltip = std::move(w.m_tooltip);

		w.m_id = 0;
	}

	Widget& Widget::operator=(Widget&& w)
	{
		if (this != &w)
		{
			this->m_id      = w.m_id;
			this->m_theme   = w.m_theme;
			this->m_tooltip = std::move(w.m_tooltip);

			w.m_id = 0;
		}

		return *this;
	}

	Widget::~Widget()
	{
		m_tooltip.reset();
		m_theme = nullptr;
	}
} // namespace galaxy