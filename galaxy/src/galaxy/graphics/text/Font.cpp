///
/// Font.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <numeric>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/error/Log.hpp"
#include "galaxy/fs/Config.hpp"
#include "galaxy/fs/FileSystem.hpp"
#include "galaxy/graphics/text/FreeType.hpp"

#include "Font.hpp"

///
/// Glyph vertex shader.
///
inline constexpr const char* const glyph_vert = R"(
	#version 450 core
	layout (location = 0) in vec4 vertex;
		
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
		io_frag_colour = vec4(1.0, 1.0, 1.0, texture(u_text, io_texels).r);
	}
)";

namespace galaxy
{
	namespace graphics
	{
		Font::Font() noexcept
			: m_height {0}
			, m_size {0}
			, m_filename {""}
		{
			m_shader.load_raw(glyph_vert, glyph_frag);
		}

		Font::Font(std::string_view filepath, const int size)
		{
			m_shader.load_raw(glyph_vert, glyph_frag);

			if (!create(filepath, size))
			{
				GALAXY_LOG(GALAXY_FATAL, "Failed to load font file: {0}.", filepath);
			}
		}

		Font::~Font() noexcept
		{
			m_characters.clear();
		}

		const bool Font::create(std::string_view file, const int size)
		{
			bool success = true;

			if (!m_characters.empty())
			{
				GALAXY_LOG(GALAXY_ERROR, "Already created font.");
				success = false;
			}
			else
			{
				const auto path = SL_HANDLE.vfs()->absolute(file);
				if (path == std::nullopt)
				{
					GALAXY_LOG(GALAXY_ERROR, "Tried to open non-existent font: {0}.", file);
					success = false;
				}
				else
				{
					m_filename = static_cast<std::string>(file);
					m_size     = size;

					FT_Face face;
					if (FT_New_Face(FT_HANDLE.lib(), path.value().c_str(), 0, &face) != FT_OK)
					{
						GALAXY_LOG(GALAXY_ERROR, "Failed to create font face for: {0}.", file);
						success = false;
					}
					else
					{
						int orig_alignment = 0;
						glGetIntegerv(GL_UNPACK_ALIGNMENT, &orig_alignment);
						glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

						GLuint char_vbo = 0;
						GLuint char_vao = 0;
						glGenVertexArrays(1, &char_vao);
						glGenBuffers(1, &char_vbo);
						glBindVertexArray(char_vao);
						glBindBuffer(GL_ARRAY_BUFFER, char_vbo);
						glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
						glEnableVertexAttribArray(0);
						glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
						glBindBuffer(GL_ARRAY_BUFFER, 0);
						glBindVertexArray(0);

						FT_Set_Pixel_Sizes(face, 0, size);

						int max_ascent  = 0;
						int max_descent = 0;
						int total_width = 0;

						FT_UInt index = 0;
						auto c        = FT_Get_First_Char(face, &index);
						while (index)
						{
							Character c_obj;
							FT_Load_Char(face, c, FT_LOAD_RENDER);

							glBindTexture(GL_TEXTURE_2D, c_obj.m_gl_texture);
							glTexImage2D(GL_TEXTURE_2D,
										 0,
										 GL_RED,
										 face->glyph->bitmap.width,
										 face->glyph->bitmap.rows,
										 0,
										 GL_RED,
										 GL_UNSIGNED_BYTE,
										 face->glyph->bitmap.buffer);

							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
							glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, static_cast<float>(SL_HANDLE.config()->get<int>("ansio-filter")));

							c_obj.m_size.x    = face->glyph->bitmap.width;
							c_obj.m_size.y    = face->glyph->bitmap.rows;
							c_obj.m_bearing.x = face->glyph->bitmap_left;
							c_obj.m_bearing.y = face->glyph->bitmap_top;
							c_obj.m_advance   = face->glyph->advance.x;

							if (face->glyph->bitmap_top > max_ascent)
							{
								max_ascent = face->glyph->bitmap_top;
							}

							if (((face->glyph->metrics.height >> 6) - face->glyph->bitmap_top) > max_descent)
							{
								max_descent = (face->glyph->metrics.height >> 6) - face->glyph->bitmap_top;
							}

							total_width += (c_obj.m_advance >> 6);

							m_characters.emplace(c, std::move(c_obj));
							c = FT_Get_Next_Char(face, c, &index);
							glBindTexture(GL_TEXTURE_2D, 0);
						}

						m_height = max_ascent + max_descent;
						glPixelStorei(GL_UNPACK_ALIGNMENT, orig_alignment);

						m_fontmap.create(total_width, m_height);
						m_fontmap.bind(true);

						m_shader.bind();
						m_shader.set_uniform("u_proj", m_fontmap.get_proj());
						glBindVertexArray(char_vao);

						float offset_x = 0.0f;
						for (auto& [c, c_obj] : m_characters)
						{
							float x = offset_x + c_obj.m_bearing.x;
							float y = m_characters['X'].m_bearing.y - c_obj.m_bearing.y;
							float w = c_obj.m_size.x;
							float h = c_obj.m_size.y;

							float vertices[6][4] = {{x, y + h, 0.0f, 1.0f},
													{x + w, y, 1.0f, 0.0f},
													{x, y, 0.0f, 0.0f},

													{x, y + h, 0.0f, 1.0f},
													{x + w, y + h, 1.0f, 1.0f},
													{x + w, y, 1.0f, 0.0f}};

							c_obj.m_region = {x, 0.0f, w, static_cast<float>(m_height)};
							glBindTexture(GL_TEXTURE_2D, c_obj.m_gl_texture);

							glBindBuffer(GL_ARRAY_BUFFER, char_vbo);
							glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

							glDrawArrays(GL_TRIANGLES, 0, 6);

							offset_x += (c_obj.m_advance >> 6);
						}

						glBindBuffer(GL_ARRAY_BUFFER, 0);
						glBindTexture(GL_TEXTURE_2D, 0);
						m_fontmap.unbind();

						glBindVertexArray(0);
						glDeleteVertexArrays(1, &char_vao);
						glDeleteBuffers(1, &char_vbo);
					}

					FT_Done_Face(face);
				}
			}

			return success;
		}

		Character* Font::get_char(char c) noexcept
		{
			if (!m_characters.contains(c))
			{
				GALAXY_LOG(GALAXY_WARNING, "FontMap does not contain character: '{0}'.", c);
				return nullptr;
			}
			else
			{
				return &m_characters[c];
			}
		}

		const unsigned int Font::get_fontmap() const noexcept
		{
			return m_fontmap.get_texture();
		}

		RenderTexture* Font::get_rendertexture() noexcept
		{
			return &m_fontmap;
		}

		const int Font::get_width(std::string_view text) noexcept
		{
			return std::accumulate(text.begin(), text.end(), 0, [&](int width, const char c) {
				return width += (m_characters[c].m_bearing.x + (m_characters[c].m_advance >> 6));
			});
		}

		const int Font::get_height() const noexcept
		{
			return m_height;
		}

		const int Font::get_pixel_size() const noexcept
		{
			return m_size;
		}

		const std::string& Font::get_filename() const noexcept
		{
			return m_filename;
		}
	} // namespace graphics
} // namespace galaxy