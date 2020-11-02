///
/// Label.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <qs/core/Renderer.hpp>

#include "Label.hpp"

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
		void Label::create(std::string_view text, std::string_view font)
		{
			auto fontptr   = m_theme->m_fonts->get(font);
			auto shaderptr = m_theme->m_shaders->get("text");
			m_text.load(*fontptr, *shaderptr, m_theme->m_font_col);
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

		void Label::on_event(const pr::MouseMovedEvent& mme)
		{
			if (m_tooltip)
			{
				if (m_bounds.contains(mme.m_x, mme.m_y))
				{
					m_tooltip->toggle_tooltip(true);
					m_tooltip->update_pos(mme.m_x, mme.m_y);
				}
				else
				{
					m_tooltip->toggle_tooltip(false);
				}
			}
		}

		void Label::update()
		{
		}

		void Label::render(qs::Camera& camera)
		{
			auto ss = m_theme->m_shaders->get("sprite");

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
	} // namespace widget
} // namespace galaxy