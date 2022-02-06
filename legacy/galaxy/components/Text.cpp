///
/// Text.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/resource/FontBook.hpp"

#include "Text.hpp"

///
/// Glyph vertex shader.
///
inline constexpr const char* const glyph_vert = R"(
	#version 450 core
	layout (location = 0) in vec4 vertex; // vec2 pos, vec2 tex.
		
	out vec2 io_texels;

	uniform mat4 u_proj;

	void main()
	{
		gl_Position = u_proj * vec4(vertex.xy, 0.0, 1.0);
		io_texels = vertex.zw;
	}
)";

///
/// Glyph fragment shader.
///
inline constexpr const char* const glyph_frag = R"(
	#version 450 core

	in vec2 io_texels;
	out vec4 io_frag_colour;

	uniform sampler2D u_text;

	void main()
	{
		io_frag_colour = vec4(0.0, 0.0, 0.0, texture(u_text, io_texels).r);
	}
)";

namespace galaxy
{
	namespace components
	{
		Text::Text() noexcept
			: Serializable {this}
			, m_colour {255, 255, 255, 255}
			, m_width {0}
			, m_height {0}
		{
			glGenVertexArrays(1, &m_glyph_vao);
			glGenBuffers(1, &m_glyph_vbo);
			glBindVertexArray(m_glyph_vao);
			glBindBuffer(GL_ARRAY_BUFFER, m_glyph_vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			m_shader.load_raw(glyph_vert, glyph_frag);
			m_text_texture.create(1, 1);
		}

		Text::Text(const nlohmann::json& json)
			: Serializable {this}
		{
			glGenVertexArrays(1, &m_glyph_vao);
			glGenBuffers(1, &m_glyph_vbo);
			glBindVertexArray(m_glyph_vao);
			glBindBuffer(GL_ARRAY_BUFFER, m_glyph_vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			m_shader.load_raw(glyph_vert, glyph_frag);
			m_text_texture.create(1, 1);

			deserialize(json);
		}

		Text::Text(Text&& t) noexcept
			: Serializable {this}
		{
			this->m_font_id      = std::move(t.m_font_id);
			this->m_colour       = std::move(t.m_colour);
			this->m_text         = std::move(t.m_text);
			this->m_layer        = std::move(t.m_layer);
			this->m_shader       = std::move(t.m_shader);
			this->m_glyph_vbo    = t.m_glyph_vbo;
			this->m_glyph_vao    = t.m_glyph_vao;
			this->m_text_texture = std::move(t.m_text_texture);
			this->m_text_vao     = std::move(t.m_text_vao);
			this->m_width        = t.m_width;
			this->m_height       = t.m_height;

			t.m_glyph_vbo = 0;
			t.m_glyph_vao = 0;
		}

		Text& Text::operator=(Text&& t) noexcept
		{
			if (this != &t)
			{
				this->m_font_id      = std::move(t.m_font_id);
				this->m_colour       = std::move(t.m_colour);
				this->m_text         = std::move(t.m_text);
				this->m_layer        = std::move(t.m_layer);
				this->m_shader       = std::move(t.m_shader);
				this->m_glyph_vbo    = t.m_glyph_vbo;
				this->m_glyph_vao    = t.m_glyph_vao;
				this->m_text_texture = std::move(t.m_text_texture);
				this->m_text_vao     = std::move(t.m_text_vao);
				this->m_width        = t.m_width;
				this->m_height       = t.m_height;

				t.m_glyph_vbo = 0;
				t.m_glyph_vao = 0;
			}

			return *this;
		}

		Text::~Text() noexcept
		{
			if (m_glyph_vao != 0)
			{
				glDeleteVertexArrays(1, &m_glyph_vao);
				m_glyph_vao = 0;
			}

			if (m_glyph_vbo != 0)
			{
				glDeleteBuffers(1, &m_glyph_vbo);
				m_glyph_vbo = 0;
			}
		}

		void Text::create(std::string_view font, const graphics::Colour& col, std::string_view text, std::string_view layer)
		{
			m_font_id = static_cast<std::string>(font);
			m_text    = static_cast<std::string>(text);
			m_layer   = static_cast<std::string>(layer);

			set_colour(col.m_red, col.m_green, col.m_blue);
			set_opacity(col.m_alpha);

			auto* const fontmap = SL_HANDLE.fontbook()->get(m_font_id);
			if (!m_text.empty() && fontmap != nullptr)
			{
				m_width  = fontmap->get_width(m_text);
				m_height = fontmap->get_height();
				m_text_texture.resize(m_width, m_height);
				m_text_texture.bind(true);

				m_shader.bind();
				m_shader.set_uniform("u_proj", m_text_texture.get_proj());
				m_shader.set_uniform("u_text", 0);

				glActiveTexture(GL_TEXTURE0);
				glBindVertexArray(m_glyph_vao);

				float      x_offset = 0.0f;
				const auto X_height = fontmap->get_char('X')->m_bearing.y;
				for (const auto& character : m_text)
				{
					auto* const font_char = fontmap->get_char(character);

					const float x = x_offset + font_char->m_bearing.x;
					const float y = static_cast<float>(X_height - font_char->m_bearing.y);

					const float vertices[6][4] = {{x, y + font_char->m_size.y, 0.0f, 1.0f},
						{x + font_char->m_size.x, y, 1.0f, 0.0f},
						{x, y, 0.0f, 0.0f},

						{x, y + font_char->m_size.y, 0.0f, 1.0f},
						{x + font_char->m_size.x, y + font_char->m_size.y, 1.0f, 1.0f},
						{x + font_char->m_size.x, y, 1.0f, 0.0f}};

					glBindTexture(GL_TEXTURE_2D, font_char->m_texture);
					glBindBuffer(GL_ARRAY_BUFFER, m_glyph_vbo);
					glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glDrawArrays(GL_TRIANGLES, 0, 6);

					x_offset += (font_char->m_advance >> 6);
				}

				glBindVertexArray(0);
				glBindTexture(GL_TEXTURE_2D, 0);
				glUseProgram(0);
				m_text_texture.unbind();

				std::array<graphics::Vertex, 4> vertices;
				vertices[0].m_pos    = {0.0f, 0.0f};
				vertices[0].m_texels = {0.0f, 0.0f};

				vertices[1].m_pos    = {0.0f + m_width, 0.0f};
				vertices[1].m_texels = {0.0f + m_width, 0.0f};

				vertices[2].m_pos    = {0.0f + m_width, 0.0f + m_height};
				vertices[2].m_texels = {0.0f + m_width, 0.0f + m_height};

				vertices[3].m_pos    = {0.0f, 0.0f + m_height};
				vertices[3].m_texels = {0.0f, 0.0f + m_height};

				std::array<unsigned int, 6> indices = {0, 1, 3, 1, 2, 3};

				graphics::VertexBuffer vbo;
				graphics::IndexBuffer  ibo;

				vbo.create(vertices, false);
				ibo.create(indices, true);

				m_text_vao.create(vbo, ibo);
			}
		}

		void Text::update(std::string_view text)
		{
			create(m_font_id, m_colour, text, m_layer);
		}

		void Text::set_colour(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b) noexcept
		{
			m_colour.m_red   = std::clamp<std::uint8_t>(r, 0, 255);
			m_colour.m_green = std::clamp<std::uint8_t>(g, 0, 255);
			m_colour.m_blue  = std::clamp<std::uint8_t>(b, 0, 255);
		}

		void Text::set_opacity(const std::uint8_t opacity) noexcept
		{
			m_colour.m_alpha = std::clamp<std::uint8_t>(opacity, 0, 255);
		}

		graphics::Colour& Text::get_colour() noexcept
		{
			return m_colour;
		}

		const std::string& Text::get_layer() const noexcept
		{
			return m_layer;
		}

		const int Text::get_width() const noexcept
		{
			return m_width;
		}

		const int Text::get_height() const noexcept
		{
			return m_height;
		}

		const int Text::index_count() const noexcept
		{
			return m_text_vao.index_count();
		}

		const unsigned int Text::vao() const noexcept
		{
			return m_text_vao.id();
		}

		const unsigned int Text::gl_texture() const noexcept
		{
			return m_text_texture.get_texture();
		}

		nlohmann::json Text::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["font"]        = m_font_id;
			json["text"]        = m_text;
			json["layer"]       = m_layer;

			json["colour"]      = nlohmann::json::object();
			json["colour"]["r"] = m_colour.m_red;
			json["colour"]["g"] = m_colour.m_green;
			json["colour"]["b"] = m_colour.m_blue;
			json["colour"]["a"] = m_colour.m_alpha;

			return json;
		}

		void Text::deserialize(const nlohmann::json& json)
		{
			const auto& colour = json.at("colour");
			create(json.at("font"), {colour.at("r"), colour.at("g"), colour.at("b"), colour.at("a")}, json.at("text"), json.at("layer"));
		}
	} // namespace components
} // namespace galaxy