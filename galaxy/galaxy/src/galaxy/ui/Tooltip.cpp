///
/// Tooltip.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <qs/core/Renderer.hpp>

#include "Tooltip.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	Tooltip::Tooltip()
	    : m_theme {nullptr}, m_ns {nullptr}
	{
	}

	void Tooltip::create(std::string_view ns_tex, const std::string& text, std::string_view font)
	{
		auto region = m_theme->m_atlas.get_region(ns_tex).value();
		m_sprite.load(m_theme->m_atlas.gl_texture(), region.m_width, region.m_height);
		m_sprite.create<qs::BufferDynamic>(region.m_x, region.m_y);

		auto* fontptr = m_theme->m_fonts->get(font);
		m_text.load(text, fontptr, m_theme->m_font_col);
		m_text.create();

		m_ns = m_theme->m_atlas.get_nine_slice(ns_tex);
	}

	void Tooltip::render(qs::Camera& camera)
	{
		auto nss = m_theme->m_shaders->get("9slice");
		auto ts  = m_theme->m_shaders->get("text");

		nss->bind();
		nss->set_uniform("u_cameraProj", camera.get_proj());
		nss->set_uniform("u_cameraView", camera.get_transform());
		nss->set_uniform<glm::vec2>("u_9grid_size", m_ns->m_cell_size);
		nss->set_uniform<glm::vec2>("u_total_area", {m_text.get_width(), m_text.get_height()});
		m_theme->m_renderer->draw_sprite(m_sprite, *nss);

		ts->bind();
		ts->set_uniform("u_cameraProj", camera.get_proj());
		ts->set_uniform("u_cameraView", camera.get_transform());
		m_theme->m_renderer->draw_text(m_text, *ts);
	}
} // namespace galaxy