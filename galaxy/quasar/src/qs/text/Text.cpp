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
	    : VertexData {}, Transform {}, m_width {0.0f}, m_height {0.0f}, m_text {""}, m_font {nullptr}
	{
		m_is.clear();
		m_vs.clear();
	}

	Text::Text(Text&& t)
	{
		this->m_width  = t.m_width;
		this->m_height = t.m_height;
		this->m_text   = std::move(t.m_text);
		this->m_font   = t.m_font;
		this->m_colour = std::move(t.m_colour);

		t.m_text = "";
		t.m_font = nullptr;
	}

	Text& Text::operator=(Text&& t)
	{
		if (this != &t)
		{
			this->m_width  = t.m_width;
			this->m_height = t.m_height;
			this->m_text   = std::move(t.m_text);
			this->m_font   = t.m_font;
			this->m_colour = std::move(t.m_colour);

			t.m_text = "";
			t.m_font = nullptr;
		}

		return *this;
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

			m_width  = m_font->get_text_width(m_text);
			m_height = m_font->get_height();
		}
	}

	void Text::create()
	{
		m_is.reserve(m_text.length() * 6);
		m_vs.reserve(m_text.length() * 4);

		unsigned int count = 0;
		float x            = 0;
		for (auto& c : m_text)
		{
			m_is.push_back(0 + count);
			m_is.push_back(1 + count);
			m_is.push_back(3 + count);
			m_is.push_back(1 + count);
			m_is.push_back(2 + count);
			m_is.push_back(3 + count);

			const auto* chr = m_font->get_char(c);
			auto* region    = &chr->get_region();
			m_vs.push_back(qs::make_vertex<qs::SpriteVertex>(x, 0.0f, region->m_x, region->m_y));
			m_vs.push_back(qs::make_vertex<qs::SpriteVertex>(x + region->m_width, 0.0f, region->m_x + region->m_width, region->m_y));
			m_vs.push_back(qs::make_vertex<qs::SpriteVertex>(x + region->m_width, 0.0f + region->m_height, region->m_x + region->m_width, region->m_y + region->m_height));
			m_vs.push_back(qs::make_vertex<qs::SpriteVertex>(x, 0.0f + region->m_height, region->m_x, region->m_y + region->m_height));

			count += 4;
			x += (chr->get_advance() >> 6);
		}

		m_vb.create<qs::SpriteVertex, qs::BufferDynamic>(m_vs);
		m_ib.create<qs::BufferStatic>(m_is);

		m_layout.add<qs::SpriteVertex, qs::VAPosition>(2);
		m_layout.add<qs::SpriteVertex, qs::VATexel>(2);

		m_va.create<qs::SpriteVertex>(m_vb, m_ib, m_layout);

		m_is.clear();
		m_vs.clear();
	}

	void Text::update_text(const std::string& text)
	{
		m_text = text;

		m_is.reserve(m_text.length() * 6);
		m_vs.reserve(m_text.length() * 4);

		unsigned int count = 0;
		float x            = 0;
		for (auto& c : m_text)
		{
			m_is.push_back(0 + count);
			m_is.push_back(1 + count);
			m_is.push_back(3 + count);
			m_is.push_back(1 + count);
			m_is.push_back(2 + count);
			m_is.push_back(3 + count);

			const auto* chr = m_font->get_char(c);
			auto* region    = &chr->get_region();
			m_vs.push_back(qs::make_vertex<qs::SpriteVertex>(x, 0.0f, region->m_x, region->m_y));
			m_vs.push_back(qs::make_vertex<qs::SpriteVertex>(x + region->m_width, 0.0f, region->m_x + region->m_width, region->m_y));
			m_vs.push_back(qs::make_vertex<qs::SpriteVertex>(x + region->m_width, 0.0f + region->m_height, region->m_x + region->m_width, region->m_y + region->m_height));
			m_vs.push_back(qs::make_vertex<qs::SpriteVertex>(x, 0.0f + region->m_height, region->m_x, region->m_y + region->m_height));

			count += 4;
			x += (chr->get_advance() >> 6);
		}

		m_vb.create<qs::SpriteVertex, qs::BufferDynamic>(m_vs);
		m_ib.create<qs::BufferStatic>(m_is);

		m_is.clear();
		m_vs.clear();

		m_width = m_font->get_text_width(m_text);
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

	const float Text::get_width() const noexcept
	{
		return m_width;
	}

	const float Text::get_height() const noexcept
	{
		return m_height;
	}
} // namespace qs