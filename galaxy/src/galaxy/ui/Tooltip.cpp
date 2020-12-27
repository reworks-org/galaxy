///
/// Tooltip.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/graphics/Renderer.hpp"

#include "Tooltip.hpp"

namespace galaxy
{
	namespace ui
	{
		Tooltip::Tooltip()
		    : m_draw {false}, m_theme {nullptr}
		{
		}

		void Tooltip::create(std::string_view sprite, const std::string& text, std::string_view font)
		{
			m_sprite.load(sprite);
			m_sprite.create<graphics::BufferStatic>();

			m_text.load(m_theme->m_fonts->get(font), m_theme->m_font_col);
			m_text.create(text);
		}

		void Tooltip::render(graphics::Camera& camera)
		{
			auto ss = m_theme->m_shaders->get("sprite");
			ss->bind();
			ss->set_uniform("u_cameraProj", camera.get_proj());
			ss->set_uniform("u_cameraView", camera.get_transform());
			m_theme->m_renderer->draw_sprite(m_sprite, *ss);

			auto ts = m_theme->m_shaders->get("text");
			ts->bind();
			ts->set_uniform("u_cameraProj", camera.get_proj());
			ts->set_uniform("u_cameraView", camera.get_transform());
			m_theme->m_renderer->draw_text(m_text, *ts);
		}

		void Tooltip::update_pos(const double x, const double y)
		{
			m_sprite.set_pos(x, y);

			float text_x = 0.0f, text_y = 0.0f;
			text_x = x + ((m_sprite.get_width() * 0.5f) - (m_text.get_width() * 0.5f));
			text_y = y + ((m_sprite.get_height() * 0.5f) - (m_text.get_height() * 0.5f));
			m_text.set_pos(text_x, text_y);
		}

		void Tooltip::can_draw(const bool can_draw)
		{
			m_draw = can_draw;
		}

		const bool Tooltip::can_draw() const
		{
			return m_draw;
		}
	} // namespace ui
} // namespace galaxy