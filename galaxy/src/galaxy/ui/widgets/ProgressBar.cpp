///
/// Progressbar.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <algorithm>

#include "Progressbar.hpp"

namespace galaxy
{
	namespace ui
	{
		Progressbar::Progressbar() noexcept
		    : m_progress {1.0f}, m_prev_progress {-1.0f}, m_bar_pos {0.0f, 0.0f, 0.0f, 0.0f}
		{
		}

		void Progressbar::create(std::string_view container, std::string_view bar)
		{
			m_container.create(container);
			m_bar.create(bar);

			m_bounds.m_width  = m_container.get_width();
			m_bounds.m_height = m_container.get_height();
		}

		void Progressbar::on_event(const events::MouseMoved& mme) noexcept
		{
			if (m_tooltip)
			{
				if (m_bounds.contains(mme.m_x, mme.m_y))
				{
					m_tooltip->can_draw(true);
					m_tooltip->update_pos(mme.m_x, mme.m_y);
				}
				else
				{
					m_tooltip->can_draw(false);
				}
			}
		}

		void Progressbar::update(const double dt)
		{
			if (m_progress != m_prev_progress)
			{
				m_bar.set_custom_width(m_bounds.m_width * m_progress);
				m_prev_progress = m_progress;
			}
		}

		void Progressbar::render()
		{
			if (m_tooltip)
			{
				if (m_tooltip->can_draw())
				{
					m_tooltip->render();
				}
			}
		}

		void Progressbar::set_pos(const float x, const float y) noexcept
		{
			m_bounds.m_x = x;
			m_bounds.m_y = y;

			m_container_transform.set_pos(x, y);
			m_bar_transform.set_pos(x, y);
		}

		void Progressbar::set_progress(const float progress) noexcept
		{
			m_progress = std::clamp(progress, 0.0f, 1.0f);
		}

		const float Progressbar::progress() const noexcept
		{
			return m_progress;
		}

		const float Progressbar::percentage() const noexcept
		{
			return m_progress * 100.0f;
		}
	} // namespace ui
} // namespace galaxy