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
	Widget::Widget()
	    : m_id {0}, m_theme {nullptr}, m_tooltip {nullptr}
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