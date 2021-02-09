///
/// ToggleButton.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/graphics/Renderer.hpp"
#include "galaxy/res/TextureAtlas.hpp"

#include "ToggleButton.hpp"

namespace galaxy
{
	namespace ui
	{
		ToggleButton::ToggleButton() noexcept
		    : m_state {ToggleButton::State::OFF}, m_on {false}, m_callback {nullptr}
		{
		}

		void ToggleButton::create(std::string_view on, std::string_view off, std::string_view on_hover, std::string_view off_hover)
		{
			m_batched.create(on);
			m_regions[0] = SL_HANDLE.atlas()->get_region(on);
			m_regions[1] = SL_HANDLE.atlas()->get_region(off);
			m_regions[2] = SL_HANDLE.atlas()->get_region(on_hover);
			m_regions[3] = SL_HANDLE.atlas()->get_region(off_hover);

			m_bounds.m_width  = m_batched.get_width();
			m_bounds.m_height = m_batched.get_height();
		}

		void ToggleButton::set_pos(const float x, const float y) noexcept
		{
			m_bounds.m_x = x;
			m_bounds.m_y = y;

			m_transform.set_pos(x, y);
		}

		void ToggleButton::on_event(const events::MouseMoved& mme) noexcept
		{
			if (m_bounds.contains(mme.m_x, mme.m_y))
			{
				if (m_on)
				{
					m_state = ToggleButton::State::ON_HOVER;
				}
				else
				{
					m_state = ToggleButton::State::OFF_HOVER;
				}

				if (m_tooltip)
				{
					m_tooltip->can_draw(true);
					m_tooltip->update_pos(mme.m_x, mme.m_y);
				}
			}
			else
			{
				if (m_on)
				{
					m_state = ToggleButton::State::ON;
				}
				else
				{
					m_state = ToggleButton::State::OFF;
				}

				if (m_tooltip)
				{
					m_tooltip->can_draw(false);
				}
			}
		}

		void ToggleButton::on_event(const events::MousePressed& mpe) noexcept
		{
			if (m_bounds.contains(mpe.m_x, mpe.m_y))
			{
				m_on = !m_on;
				if (m_on)
				{
					m_state = ToggleButton::State::ON_HOVER;
				}
				else
				{
					m_state = ToggleButton::State::OFF_HOVER;
				}

				if (m_callback != nullptr)
				{
					m_callback(m_on);
				}

				if (m_sound != nullptr)
				{
					m_sound->play();
				}

				if (m_tooltip)
				{
					m_tooltip->can_draw(false);
				}
			}
		}

		void ToggleButton::update(const double dt)
		{
			switch (m_state)
			{
				case ToggleButton::State::ON:
					m_batched.update_region(m_regions[0]);
					break;

				case ToggleButton::State::OFF:
					m_batched.update_region(m_regions[1]);
					break;

				case ToggleButton::State::ON_HOVER:
					m_batched.update_region(m_regions[2]);
					break;

				case ToggleButton::State::OFF_HOVER:
					m_batched.update_region(m_regions[3]);
					break;
			}
		}

		void ToggleButton::render()
		{
			if (m_tooltip)
			{
				if (m_tooltip->can_draw())
				{
					m_tooltip->render();
				}
			}
		}

		const bool ToggleButton::is_on() const noexcept
		{
			return m_on;
		}
	} // namespace ui
} // namespace galaxy