///
/// Font.cpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include <filesystem>

#include "qs/utils/Error.hpp"

#include "Font.hpp"

///
/// Core namespace.
///
namespace qs
{
	Font::Font() noexcept
		:m_height(0)
	{
		m_characterMap.reserve(128);
	}

	Font::Font(const std::string& file, const int size) noexcept
		:m_height(0)
	{
		m_characterMap.reserve(128);

		create(file, size);
	}

	void Font::create(const std::string& file, const int size) noexcept
	{
		FT_Face face;
		auto path = std::filesystem::path(file);

		if (FT_New_Face(qs::FreeTypeLib::lib(), path.string().c_str(), 0, &face))
		{
			qs::Error::handle().callback("Font.cpp", 44, "Failed to create FreeType font face.");
		}
		else
		{
			// Modify alignment for fonts.
			int original = 0;
			glGetIntegerv(GL_UNPACK_ALIGNMENT, &original);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			
			FT_Set_Pixel_Sizes(face, 0, size);
			for (GLubyte chr = 0; chr < 128; chr++)
			{
				if (FT_Load_Char(face, chr, FT_LOAD_RENDER))
				{
					qs::Error::handle().callback("Font.cpp", 58, "Failed to load character: " + chr);
				}
				else
				{
                    GLuint texture;
                    glGenTextures(1, &texture);
                    glBindTexture(GL_TEXTURE_2D, texture);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					
					m_characterMap.emplace(chr, qs::Character{ texture, static_cast<int>(face->glyph->bitmap.width), static_cast<int>(face->glyph->bitmap.rows), static_cast<int>(face->glyph->bitmap_left), static_cast<int>(face->glyph->bitmap_top), static_cast<unsigned int>(face->glyph->advance.x) });
				}
			}

			// Restore alignment.
			glPixelStorei(GL_UNPACK_ALIGNMENT, original);
		}

		FT_Done_Face(face);
		m_height = m_characterMap['X'].m_height;
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

	std::unordered_map<char, qs::Character>& Font::getChars() noexcept
	{
		return m_characterMap;
	}
}