///
/// Font.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include <filesystem>

#include <pulsar/Log.hpp>

#include "qs/core/Shader.hpp"
#include "qs/core/Renderer.hpp"

#include "Font.hpp"

///
/// Core namespace.
///
namespace qs
{
	Font::Font() noexcept
	    : m_height {0}, m_texture {}
	{
	}

	Font::Font(std::string_view file, const unsigned int size)
	    : m_height {0}, m_texture {}
	{
		load(file, size);
	}

	void Font::load(std::string_view file, const unsigned int size)
	{
		FT_Face face;
		auto path = std::filesystem::path {file};

		if (FT_New_Face(FTLIB.lib(), path.string().c_str(), 0, &face))
		{
			PL_LOG(PL_ERROR, "Failed to create FreeType font face for file: {0}.", path.string());
		}
		else
		{
			float adv_x = 0;
			FT_Set_Pixel_Sizes(face, 0, size);
			for (GLubyte chr = 0; chr < 128; chr++)
			{
				if (FT_Load_Char(face, chr, FT_LOAD_RENDER))
				{
					PL_LOG(PL_ERROR, "Failed to load character: {0}.", chr);
				}
				else
				{
					// This will default construct the object.
					qs::Character* emplaced = &m_characters[chr];

					// Modify alignment for fonts.
					int original = 0;
					glGetIntegerv(GL_UNPACK_ALIGNMENT, &original);
					glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

					emplaced->load(0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
					emplaced->m_bearing_x = static_cast<int>(face->glyph->bitmap_left);
					emplaced->m_bearing_y = static_cast<int>(face->glyph->bitmap_top);
					emplaced->m_advance   = static_cast<unsigned int>(face->glyph->advance.x);

					float x       = adv_x + emplaced->m_bearing_x;
					float y       = static_cast<float>(0 - (emplaced->get_height() - emplaced->m_bearing_y));
					const float w = static_cast<float>(emplaced->get_width());
					const float h = static_cast<float>(emplaced->get_height());

					// Restore alignment.
					glPixelStorei(GL_UNPACK_ALIGNMENT, original);

					auto v1 = qs::make_vertex<qs::SpriteVertex>(x, y, 0.0f, 1.0f, 1.0f);
					auto v2 = qs::make_vertex<qs::SpriteVertex>(x + w, y, 1.0f, 1.0f, 1.0f);
					auto v3 = qs::make_vertex<qs::SpriteVertex>(x + w, y + h, 1.0f, 0.0f, 1.0f);
					auto v4 = qs::make_vertex<qs::SpriteVertex>(x, y + h, 0.0f, 0.0f, 1.0f);

					emplaced->m_region = {x, y, w, h};

					std::array<qs::SpriteVertex, 4> arr_vb = {v1, v2, v3, v4};
					emplaced->m_vb.create<qs::SpriteVertex, qs::BufferStatic>(arr_vb);

					std::array<unsigned int, 6> arr_ib = {0, 1, 3, 1, 2, 3};
					emplaced->m_ib.create<qs::BufferStatic>(arr_ib);

					emplaced->m_layout.add<qs::SpriteVertex, qs::VAPosition>(2);
					emplaced->m_layout.add<qs::SpriteVertex, qs::VATexel>(2);
					emplaced->m_layout.add<qs::SpriteVertex, qs::VAOpacity>(1);

					emplaced->m_va.create<qs::SpriteVertex>(emplaced->m_vb, emplaced->m_ib, emplaced->m_layout);

					// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
					adv_x += (emplaced->get_advance() >> 6); // Bitshift by 6 to get value in pixels (2^6 = 64)
				}
			}
		}

		FT_Done_Face(face);
		m_height = m_characters['X'].get_height();
	}

	void Font::create(qs::Renderer& renderer, qs::Shader& shader)
	{
		int width = 0;
		for (auto& pair : m_characters)
		{
			width += (pair.second.get_advance() >> 6);
		}

		m_texture.create(width, m_height);
		m_texture.bind();

		for (auto& pair : m_characters)
		{
			renderer.draw_character(&pair.second, m_texture, shader);
		}

		m_texture.unbind();
	}

	const int Font::get_text_width(const std::string& text) noexcept
	{
		int width = 0;

		for (auto& chr : text)
		{
			width += (m_characters[chr].get_advance() >> 6);
		}

		return width;
	}

	const int Font::get_height() noexcept
	{
		return m_height;
	}

	qs::BaseTexture* Font::get_texture() noexcept
	{
		return dynamic_cast<qs::BaseTexture*>(&m_texture);
	}

	qs::Character* Font::get_char(const char c) noexcept
	{
		return &m_characters[c];
	}
} // namespace qs