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
			// Modify alignment for fonts.
			int original = 0;
			glGetIntegerv(GL_UNPACK_ALIGNMENT, &original);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			float advX = 0;
			FT_Set_Pixel_Sizes(face, 0, size);
			for (GLubyte chr = 0; chr < 128; chr++)
			{
				if (FT_Load_Char(face, chr, FT_LOAD_RENDER))
				{
					PL_LOG(PL_ERROR, "Failed to load character: " + chr);
				}
				else
				{
					m_characterMap.emplace(std::piecewise_construct, std::forward_as_tuple(chr), std::forward_as_tuple());
					qs::Character* emplaced = &m_characterMap[chr];

					emplaced->load(0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
					emplaced->m_bearingX = static_cast<int>(face->glyph->bitmap_left);
					emplaced->m_bearingY = static_cast<int>(face->glyph->bitmap_top);
					emplaced->m_advance = static_cast<unsigned int>(face->glyph->advance.x);
					
					emplaced->m_region.m_x = advX + emplaced->m_bearingX;
					emplaced->m_region.m_y = static_cast<float>(0 - (emplaced->getHeight() - emplaced->m_bearingY));
					emplaced->m_region.m_width = static_cast<float>(emplaced->getWidth());
					emplaced->m_region.m_height = static_cast<float>(emplaced->getHeight());

					auto v1 = qs::make_vertex<qs::SpriteVertex>(emplaced->m_region.m_x, emplaced->m_region.m_y, 0.0f, 1.0f, 1.0f);
					auto v2 = qs::make_vertex<qs::SpriteVertex>(emplaced->m_region.m_x + emplaced->m_region.m_width, emplaced->m_region.m_y, 1.0f, 1.0f, 1.0f);
					auto v3 = qs::make_vertex<qs::SpriteVertex>(emplaced->m_region.m_x + emplaced->m_region.m_width, emplaced->m_region.m_y + emplaced->m_region.m_height, 1.0f, 0.0f, 1.0f);
					auto v4 = qs::make_vertex<qs::SpriteVertex>(emplaced->m_region.m_x, emplaced->m_region.m_y + emplaced->m_region.m_height, 0.0f, 0.0f, 1.0f);

					emplaced->m_vertexBuffer.create<qs::SpriteVertex, qs::BufferTypeStatic>({ v1, v2, v3, v4 });
					emplaced->m_indexBuffer.create<qs::BufferTypeStatic>({ 0, 1, 3, 1, 2, 3 });

					emplaced->m_layout.add<qs::SpriteVertex, qs::VATypePosition>(2);
					emplaced->m_layout.add<qs::SpriteVertex, qs::VATypeTexel>(2);
					emplaced->m_layout.add<qs::SpriteVertex, qs::VATypeOpacity>(1);

					emplaced->m_vertexArray.create<qs::SpriteVertex>(emplaced->m_vertexBuffer, emplaced->m_indexBuffer, emplaced->m_layout);

					// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
					advX += (emplaced->m_advance >> 6); // Bitshift by 6 to get value in pixels (2^6 = 64)
				}
			}

			// Restore alignment.
			glPixelStorei(GL_UNPACK_ALIGNMENT, original);
		}

		FT_Done_Face(face);
		m_height = m_characterMap['X'].getHeight();
	}

	void Font::create(qs::Renderer& renderer, qs::Shader& shader) noexcept
	{
		int width = 0;
		for (auto& pair : m_characterMap)
		{
			width += (pair.second.m_advance >> 6);
		}

		m_texture.create(width, m_height);
		m_texture.bind();

		for (auto& pair : m_characterMap)
		{
			renderer.drawCharacter(pair.second, m_texture, shader);
		}

		m_texture.unbind();
	}

	const int Font::getTextWidth(const std::string& text) noexcept
	{
		int width = 0;

		for (auto& chr : text)
		{
			width += (m_characterMap[chr].m_advance >> 6);
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

	std::unordered_map<char, qs::Character>& Font::getChars() noexcept
	{
		return m_characterMap;
	}
}