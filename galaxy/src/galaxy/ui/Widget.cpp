///
/// Widget.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/res/SoundBook.hpp"

#include "Widget.hpp"

namespace galaxy
{
	namespace ui
	{
		void Widget::add_sfx(std::string_view sound)
		{
			m_sound = SL_HANDLE.soundbook()->get(sound);
		}

		const unsigned int Widget::id() const noexcept
		{
			return m_id;
		}

		Widget::Widget() noexcept
		    : m_id {0}, m_theme {nullptr}, m_tooltip {nullptr}, m_bounds {0.0f, 0.0f, 0.0f, 0.0f}, m_sound {nullptr}
		{
		}

		Widget::Widget(Widget&& w) noexcept
		{
			this->m_id      = w.m_id;
			this->m_theme   = w.m_theme;
			this->m_tooltip = std::move(w.m_tooltip);
			this->m_sound   = w.m_sound;

			w.m_id = 0;
		}

		Widget& Widget::operator=(Widget&& w) noexcept
		{
			if (this != &w)
			{
				this->m_id      = w.m_id;
				this->m_theme   = w.m_theme;
				this->m_tooltip = std::move(w.m_tooltip);
				this->m_sound   = w.m_sound;

				w.m_id = 0;
			}

			return *this;
		}

		Widget::~Widget()
		{
			m_sound = nullptr;

			m_tooltip.reset();
			m_theme = nullptr;
		}
	} // namespace ui
} // namespace galaxy