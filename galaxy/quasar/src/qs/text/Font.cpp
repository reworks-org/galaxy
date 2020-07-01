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
		:m_height(0), m_texture()
	{
	}

	Font::Font(const std::string& file, const int size) noexcept
		:m_height(0), m_texture()
	{
		load(file, size);
	}

	void Font::load(const std::string& file, const int size) noexcept
	{
		FT_Face face;
		auto path = std::filesystem::path(file);

		if (FT_New_Face(FTLIB.lib(), path.string().c_str(), 0, &face))
		{
			PL_LOG(PL_ERROR, "Failed to create FreeType font face.");
		}
		else
		{
			float advX = 0;
			FT_Set_Pixel_Sizes(face, 0, size);
			for (GLubyte chr = 0; chr < 128; chr++)
			{
				if (FT_Load_Char(face, chr, FT_LOAD_RENDER))
				{
					PL_LOG(PL_ERROR, "Failed to load character: " + std::to_string(chr));
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
					emplaced->m_bearingX = static_cast<int>(face->glyph->bitmap_left);
					emplaced->m_bearingY = static_cast<int>(face->glyph->bitmap_top);
					emplaced->m_advance = static_cast<unsigned int>(face->glyph->advance.x);
					
					float x = advX + emplaced->m_bearingX;
					float y = static_cast<float>(0 - (emplaced->getHeight() - emplaced->m_bearingY));
					const float w = static_cast<float>(emplaced->getWidth());
					const float h = static_cast<float>(emplaced->getHeight());

					// Restore alignment.
					glPixelStorei(GL_UNPACK_ALIGNMENT, original);

					auto v1 = qs::make_vertex<qs::SpriteVertex>(x, y, 0.0f, 1.0f, 1.0f);
					auto v2 = qs::make_vertex<qs::SpriteVertex>(x + w, y, 1.0f, 1.0f, 1.0f);
					auto v3 = qs::make_vertex<qs::SpriteVertex>(x + w, y + h, 1.0f, 0.0f, 1.0f);
					auto v4 = qs::make_vertex<qs::SpriteVertex>(x, y + h, 0.0f, 0.0f, 1.0f);

					emplaced->m_region = { x, y, w, h };
					emplaced->m_vertexBuffer.create<qs::SpriteVertex, qs::BufferTypeStatic>({ v1, v2, v3, v4 });
					emplaced->m_indexBuffer.create<qs::BufferTypeStatic>({ 0, 1, 3, 1, 2, 3 });

					emplaced->m_layout.add<qs::SpriteVertex, qs::VATypePosition>(2);
					emplaced->m_layout.add<qs::SpriteVertex, qs::VATypeTexel>(2);
					emplaced->m_layout.add<qs::SpriteVertex, qs::VATypeOpacity>(1);

					emplaced->m_vertexArray.create<qs::SpriteVertex>(emplaced->m_vertexBuffer, emplaced->m_indexBuffer, emplaced->m_layout);

					// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
					advX += (emplaced->getAdvance() >> 6); // Bitshift by 6 to get value in pixels (2^6 = 64)
				}
			}
		}

		FT_Done_Face(face);
		m_height = m_characters['X'].getHeight();
	}

	void Font::create(qs::Renderer& renderer, qs::Shader& shader) noexcept
	{
		int width = 0;
		for (auto& pair : m_characters)
		{
			width += (pair.second.getAdvance() >> 6);
		}

		m_texture.create(width, m_height);
		m_texture.bind();

		for (auto& pair : m_characters)
		{
			renderer.drawCharacter(&pair.second, m_texture, shader);
		}

		m_texture.unbind();
	}

	const int Font::getTextWidth(const std::string& text) noexcept
	{
		int width = 0;

		for (auto& chr : text)
		{
			width += (m_characters[chr].getAdvance() >> 6);
		}

		return width;
	}

	const int Font::getHeight() noexcept
	{
		return m_height;
	}

	qs::BaseTexture* Font::getTexture() noexcept
	{
		return dynamic_cast<qs::BaseTexture*>(&m_texture);
	}

	qs::Character* Font::getChar(const char c) noexcept
	{
		return &m_characters[c];
	}
}