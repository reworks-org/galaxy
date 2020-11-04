///
/// Progressbar.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <algorithm>

#include "Progressbar.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Widget specific namespace.
	///
	namespace widget
	{
		Progressbar::Progressbar()
		    : m_progress {1.0f}, m_prev_progress {-1.0f}, m_bar_pos {0.0f, 0.0f, 0.0f, 0.0f}
		{
		}

		void Progressbar::create(std::string_view container, std::string_view bar)
		{
			auto container_opt = m_theme->m_atlas.get_region(container);
			auto bar_opt       = m_theme->m_atlas.get_region(bar);

			if (container_opt != std::nullopt)
			{
				m_container.create(container_opt.value(), 0);
			}
			else
			{
				PL_LOG(PL_ERROR, "Unable to create container widget from {0}.", container);
			}

			m_bounds.m_width  = m_container.get_width();
			m_bounds.m_height = m_container.get_height();

			if (bar_opt != std::nullopt)
			{
				m_bar.create(bar_opt.value(), 1);
			}
			else
			{
				PL_LOG(PL_ERROR, "Unable to create container bar from {0}.", bar);
			}
		}

		void Progressbar::on_event(const pr::MouseMovedEvent& mme)
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

		void Progressbar::update()
		{
			if (m_progress != m_prev_progress)
			{
				m_bar.update_w_region(m_bounds.m_width * m_progress);
				m_prev_progress = m_progress;
			}
		}

		void Progressbar::render(qs::Camera& camera)
		{
			if (m_tooltip)
			{
				if (m_tooltip->can_draw())
				{
					m_tooltip->render(camera);
				}
			}
		}

		void Progressbar::set_pos(const float x, const float y)
		{
			m_bounds.m_x = x;
			m_bounds.m_y = y;

			m_container.set_pos(x, y);
			m_bar.set_pos(x, y);
		}

		void Progressbar::set_progress(const float progress)
		{
			m_progress = std::clamp(progress, 0.0f, 1.0f);
		}

		const float Progressbar::progress() const
		{
			return m_progress;
		}

		const float Progressbar::percentage() const
		{
			return m_progress * 100.0f;
		}
	} // namespace widget
} // namespace galaxy