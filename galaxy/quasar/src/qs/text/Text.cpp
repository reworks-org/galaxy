///
/// Text.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include "qs/core/Window.hpp"
#include "qs/core/Shader.hpp"
#include "qs/vertex/VertexArray.hpp"
#include "qs/vertex/VertexLayout.hpp"

#include "Text.hpp"

///
/// Core namespace.
///
namespace qs
{
	Text::Text() noexcept
	    : VertexData {}, Transform {}, m_text {""}, m_font {nullptr}, m_opacity {1.0f}
	{
	}

	void Text::load(const std::string& text, qs::Font* font, const pr::Colour& col) noexcept
	{
		if (font == nullptr)
		{
			PL_LOG(PL_ERROR, "Text tried to load a nullptr font.");
		}
		else
		{
			m_text   = text;
			m_font   = font;
			m_colour = col;
		}
	}

	void Text::create()
	{
		std::vector<unsigned int> is;
		std::vector<qs::SpriteVertex> vs;

		unsigned int count = 0;
		float x            = 0;
		for (auto& c : m_text)
		{
			is.push_back(0 + count);
			is.push_back(1 + count);
			is.push_back(3 + count);
			is.push_back(1 + count);
			is.push_back(2 + count);
			is.push_back(3 + count);

			const auto* chr = m_font->get_char(c);
			auto* region    = &chr->get_region();
			vs.push_back(qs::make_vertex<qs::SpriteVertex>(x, 0, region->m_x, region->m_y, 1.0f));
			vs.push_back(qs::make_vertex<qs::SpriteVertex>(x + region->m_width, 0, region->m_x + region->m_width, region->m_y, 1.0f));
			vs.push_back(qs::make_vertex<qs::SpriteVertex>(x + region->m_width, 0 + region->m_height, region->m_x + region->m_width, region->m_y + region->m_height, 1.0f));
			vs.push_back(qs::make_vertex<qs::SpriteVertex>(x, 0 + region->m_height, region->m_x, region->m_y + region->m_height, 1.0f));

			count += 4;
			x += (chr->get_advance() >> 6);
		}

		m_vb.create<qs::SpriteVertex, qs::BufferDynamic>(vs);
		m_ib.create<qs::BufferStatic>(is);

		m_layout.add<qs::SpriteVertex, qs::VAPosition>(2);
		m_layout.add<qs::SpriteVertex, qs::VATexel>(2);
		m_layout.add<qs::SpriteVertex, qs::VAOpacity>(1);

		m_va.create<qs::SpriteVertex>(m_vb, m_ib, m_layout);
	}

	void Text::update_text(const std::string& text)
	{
		m_text = text;

		std::vector<unsigned int> is;
		std::vector<qs::SpriteVertex> vs;

		unsigned int count = 0;
		float x            = 0;
		for (auto& c : m_text)
		{
			is.push_back(0 + count);
			is.push_back(1 + count);
			is.push_back(3 + count);
			is.push_back(1 + count);
			is.push_back(2 + count);
			is.push_back(3 + count);

			const auto* chr = m_font->get_char(c);
			auto* region    = &chr->get_region();
			vs.push_back(qs::make_vertex<qs::SpriteVertex>(x, 0, region->m_x, region->m_y, 1.0f));
			vs.push_back(qs::make_vertex<qs::SpriteVertex>(x + region->m_width, 0, region->m_x + region->m_width, region->m_y, 1.0f));
			vs.push_back(qs::make_vertex<qs::SpriteVertex>(x + region->m_width, 0 + region->m_height, region->m_x + region->m_width, region->m_y + region->m_height, 1.0f));
			vs.push_back(qs::make_vertex<qs::SpriteVertex>(x, 0 + region->m_height, region->m_x, region->m_y + region->m_height, 1.0f));

			count += 4;
			x += (chr->get_advance() >> 6);
		}

		m_vb.create<qs::SpriteVertex, qs::BufferDynamic>(vs);
		m_ib.create<qs::BufferStatic>(is);
	}

	void Text::set_opacity(const pr::from_0_to_1 auto opacity) noexcept
	{
		m_opacity = opacity;
	}

	const float Text::opacity() const noexcept
	{
		return m_opacity;
	}

	void Text::bind() noexcept
	{
		m_va.bind();
		glBindTexture(GL_TEXTURE_2D, m_font->get_texture()->gl_texture());
	}

	void Text::unbind() noexcept
	{
		m_va.unbind();
		glBindTexture(GL_TEXTURE_2D, 0);
	}
} // namespace qs