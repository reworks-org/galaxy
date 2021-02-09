///
/// Button.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/graphics/Renderer.hpp"
#include "galaxy/res/TextureAtlas.hpp"

#include "Button.hpp"

namespace galaxy
{
	namespace ui
	{
		Button::Button() noexcept
		    : m_state {Button::State::DEFAULT}, m_callback {nullptr}
		{
		}

		void Button::create(std::string_view def, std::string_view pressed, std::string_view hover)
		{
			m_batched.create(def);
			m_regions[0] = SL_HANDLE.atlas()->get_region(def);
			m_regions[1] = SL_HANDLE.atlas()->get_region(pressed);
			m_regions[2] = SL_HANDLE.atlas()->get_region(hover);

			m_bounds.m_width  = m_batched.get_width();
			m_bounds.m_height = m_batched.get_height();
		}

		void Button::set_pos(const float x, const float y) noexcept
		{
			m_bounds.m_x = x;
			m_bounds.m_y = y;
			m_transform.set_pos(x, y);
		}

		void Button::on_event(const events::MouseMoved& mme) noexcept
		{
			if (m_bounds.contains(mme.m_x, mme.m_y))
			{
				m_state = Button::State::HOVER;

				if (m_tooltip)
				{
					m_tooltip->can_draw(true);
					m_tooltip->update_pos(mme.m_x, mme.m_y);
				}
			}
			else
			{
				m_state = Button::State::DEFAULT;

				if (m_tooltip)
				{
					m_tooltip->can_draw(false);
				}
			}
		}

		void Button::on_event(const events::MousePressed& mpe) noexcept
		{
			if (m_bounds.contains(mpe.m_x, mpe.m_y))
			{
				m_state = Button::State::PRESSED;
				if (m_callback != nullptr)
				{
					m_callback();
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

		void Button::on_event(const events::MouseReleased& mre) noexcept
		{
			if (m_bounds.contains(mre.m_x, mre.m_y))
			{
				m_state = Button::State::HOVER;

				if (m_tooltip)
				{
					m_tooltip->can_draw(true);
					m_tooltip->update_pos(mre.m_x, mre.m_y);
				}
			}
			else
			{
				m_state = Button::State::DEFAULT;

				if (m_tooltip)
				{
					m_tooltip->can_draw(false);
				}
			}
		}

		void Button::update(const double dt)
		{
			switch (m_state)
			{
				case Button::State::DEFAULT:
					m_batched.update_region(m_regions[0]);
					break;

				case Button::State::PRESSED:
					m_batched.update_region(m_regions[1]);
					break;

				case Button::State::HOVER:
					m_batched.update_region(m_regions[2]);
					break;
			}
		}

		void Button::render()
		{
			if (m_tooltip)
			{
				if (m_tooltip->can_draw())
				{
					m_tooltip->render();
				}
			}
		}
	} // namespace ui
} // namespace galaxy