///
/// Image.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Image.hpp"

namespace galaxy
{
	namespace ui
	{
		void Image::create(std::string_view name)
		{
			m_batched.create(name);

			m_bounds.m_width  = m_batched.get_width();
			m_bounds.m_height = m_batched.get_height();
		}

		void Image::set_pos(const float x, const float y) noexcept
		{
			m_bounds.m_x = x;
			m_bounds.m_y = y;
			m_transform.set_pos(x, y);
		}

		void Image::on_event(const events::MouseMoved& mme) noexcept
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

		void Image::update(const double dt)
		{
		}

		void Image::render(graphics::Camera& camera)
		{
			if (m_tooltip)
			{
				if (m_tooltip->can_draw())
				{
					m_tooltip->render(camera);
				}
			}
		}
	} // namespace ui
} // namespace galaxy