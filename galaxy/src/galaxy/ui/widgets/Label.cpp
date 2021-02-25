///
/// Label.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/graphics/Renderer.hpp"
#include "galaxy/res/ShaderBook.hpp"

#include "Label.hpp"

namespace galaxy
{
	namespace ui
	{
		Label::Label() noexcept
		    : m_shader {nullptr}
		{
		}

		Label::~Label() noexcept
		{
			m_shader = nullptr;
		}

		void Label::create(std::string_view text, std::string_view font)
		{
			m_text.load(font, m_theme->m_font_col);
			m_text.create(text);

			m_bounds.m_width  = m_text.get_width();
			m_bounds.m_height = m_text.get_height();

			m_shader = SL_HANDLE.shaderbook()->get("text");
		}

		void Label::update_text(std::string_view text)
		{
			m_text.create(text);

			m_bounds.m_width  = m_text.get_width();
			m_bounds.m_height = m_text.get_height();
		}

		void Label::on_event(const events::MouseMoved& mme) noexcept
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

		void Label::update(const double dt)
		{
		}

		void Label::render()
		{
			m_shader->bind();
			m_shader->set_uniform("u_cameraProj", m_theme->m_camera.get_proj());
			m_shader->set_uniform("u_cameraView", m_theme->m_camera.get_transform());
			graphics::Renderer::submit_text(&m_text, &m_transform, m_shader);

			if (m_tooltip)
			{
				if (m_tooltip->can_draw())
				{
					m_tooltip->render();
				}
			}
		}

		void Label::set_pos(const float x, const float y) noexcept
		{
			m_bounds.m_x = x;
			m_bounds.m_y = y;
			m_transform.set_pos(x, y);
		}
	} // namespace ui
} // namespace galaxy