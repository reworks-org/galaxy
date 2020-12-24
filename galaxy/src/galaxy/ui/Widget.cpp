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
	void Widget::add_sfx(std::string_view sound)
	{
		m_sound = std::make_unique<frb::Sound>();
		m_sound->load(sound);
	}

	void Widget::add_sfx(std::unique_ptr<frb::Sound>&& sound)
	{
		m_sound = std::move(sound);
	}

	const unsigned int Widget::id() const
	{
		return m_id;
	}

	Widget::Widget()
	    : m_id {0}, m_theme {nullptr}, m_tooltip {nullptr}, m_bounds {0.0f, 0.0f, 0.0f, 0.0f}, m_sound {nullptr}
	{
	}

	Widget::Widget(Widget&& w)
	{
		this->m_id      = w.m_id;
		this->m_theme   = w.m_theme;
		this->m_tooltip = std::move(w.m_tooltip);
		this->m_sound   = std::move(w.m_sound);

		w.m_id = 0;
	}

	Widget& Widget::operator=(Widget&& w)
	{
		if (this != &w)
		{
			this->m_id      = w.m_id;
			this->m_theme   = w.m_theme;
			this->m_tooltip = std::move(w.m_tooltip);
			this->m_sound   = std::move(w.m_sound);

			w.m_id = 0;
		}

		return *this;
	}

	Widget::~Widget()
	{
		m_sound.reset();
		m_sound = nullptr;

		m_tooltip.reset();
		m_theme = nullptr;
	}
} // namespace galaxy