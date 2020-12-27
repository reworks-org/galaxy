///
/// Label.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/graphics/Renderer.hpp"

#include "Label.hpp"

namespace galaxy
{
	namespace ui
	{
		void Label::create(std::string_view text, std::string_view font)
		{
			m_text.load(m_theme->m_fonts->get(font), m_theme->m_font_col);
			m_text.create(text);

			m_bounds.m_width  = m_text.get_width();
			m_bounds.m_height = m_text.get_height();
		}

		void Label::update_text(std::string_view text)
		{
			m_text.update_text(text);

			m_bounds.m_width  = m_text.get_width();
			m_bounds.m_height = m_text.get_height();
		}

		void Label::on_event(const events::MouseMoved& mme)
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

		void Label::render(graphics::Camera& camera)
		{
			auto ss = m_theme->m_shaders->get("text");

			ss->bind();
			ss->set_uniform("u_cameraProj", camera.get_proj());
			ss->set_uniform("u_cameraView", camera.get_transform());
			m_theme->m_renderer->draw_text(m_text, *ss);

			if (m_tooltip)
			{
				if (m_tooltip->can_draw())
				{
					m_tooltip->render(camera);
				}
			}
		}

		void Label::set_pos(const float x, const float y)
		{
			m_bounds.m_x = x;
			m_bounds.m_y = y;

			m_text.set_pos(x, y);
		}
	} // namespace ui
} // namespace galaxy