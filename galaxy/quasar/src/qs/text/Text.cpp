///
/// Text.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>

#include "Text.hpp"

///
/// Core namespace.
///
namespace qs
{
	Text::Text()
	    : m_font {nullptr}, m_shader {nullptr}
	{
	}

	Text::Text(Text&& t)
	{
		this->m_colour = std::move(t.m_colour);
		this->m_font   = t.m_font;
		this->m_shader = t.m_shader;
		this->m_text   = std::move(t.m_text);

		t.m_font   = nullptr;
		t.m_shader = nullptr;
	}

	Text& Text::operator=(Text&& t)
	{
		if (this != &t)
		{
			this->m_colour = std::move(t.m_colour);
			this->m_font   = t.m_font;
			this->m_shader = t.m_shader;
			this->m_text   = std::move(t.m_text);

			t.m_font   = nullptr;
			t.m_shader = nullptr;
		}

		return *this;
	}

	void Text::load(Font& font, Shader& shader, const pr::Colour& col)
	{
		m_font   = &font;
		m_shader = &shader;
		m_colour = col;
	}

	void Text::create(std::string_view text)
	{
		int glyph_x = 0;
		GLuint vao  = 0;
		GLuint vbo  = 0;

		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

		m_text.create(m_font->get_width(text), m_font->get_height());
		m_text.bind();
		m_shader->bind();

		auto col = m_colour.get_normalized();
		m_shader->set_uniform("u_colour", col[0], col[1], col[2]);
		m_shader->set_uniform("u_proj", m_text.get_proj());

		std::for_each(text.begin(), text.end(), [&](const char c) {
			Character* c_obj = m_font->get_char(c);

			float x = glyph_x + c_obj->m_bearing.x;
			float y = (m_font->get_height() - c_obj->m_bearing.y);
			float w = c_obj->m_size.x;
			float h = c_obj->m_size.y;

			float vertices[6][4] = {
			    {x, y + h, 0.0f, 1.0f},
			    {x + w, y, 1.0f, 0.0f},
			    {x, y, 0.0f, 0.0f},

			    {x, y + h, 0.0f, 1.0f},
			    {x + w, y + h, 1.0f, 1.0f},
			    {x + w, y, 1.0f, 0.0f}};

			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

			glBindTexture(GL_TEXTURE_2D, c_obj->m_gl_texture);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			glyph_x += (c_obj->m_advance >> 6);
		});

		m_text.unbind();
		m_shader->unbind();
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);

		auto v1 = qs::make_vertex<qs::SpriteVertex>(0.0f, 0.0f, 0.0f, 0.0f);
		auto v2 = qs::make_vertex<qs::SpriteVertex>(0.0f + m_text.get_width(), 0.0f, 0.0f + m_text.get_width(), 0.0f);
		auto v3 = qs::make_vertex<qs::SpriteVertex>(0.0f + m_text.get_width(), 0.0f + m_text.get_height(), 0.0f + m_text.get_width(), 0.0f + m_text.get_height());
		auto v4 = qs::make_vertex<qs::SpriteVertex>(0.0f, 0.0f + m_text.get_height(), 0.0f, 0.0f + m_text.get_height());

		std::array<unsigned int, 6> ib_arr   = {0, 1, 3, 1, 2, 3};
		std::vector<qs::SpriteVertex> vb_arr = {v1, v2, v3, v4};

		m_vb.create<qs::SpriteVertex, qs::BufferDynamic>(vb_arr);
		m_ib.create<qs::BufferStatic>(ib_arr);

		m_layout.add<qs::SpriteVertex, qs::VAPosition>(2);
		m_layout.add<qs::SpriteVertex, qs::VATexel>(2);

		m_va.create<qs::SpriteVertex>(m_vb, m_ib, m_layout);

		set_rotation_origin(m_text.get_width() * 0.5f, m_text.get_height() * 0.5f);
		m_dirty = true;
	}

	void Text::update_text(std::string_view text)
	{
	}

	void Text::bind()
	{
		m_va.bind();
		glBindTexture(GL_TEXTURE_2D, m_text.gl_texture());
	}

	void Text::unbind()
	{
		m_va.unbind();
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	const int Text::get_width() const
	{
		return m_text.get_width();
	}

	const int Text::get_height() const
	{
		return m_text.get_height();
	}
} // namespace qs