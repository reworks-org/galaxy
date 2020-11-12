///
/// Font.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include <filesystem>

#include <glad/glad.h>
#include <pulsar/Log.hpp>

#include "qs/text/FreeType.hpp"

#include "Font.hpp"

///
/// Core namespace.
///
namespace qs
{
	Font::Font()
	    : m_height {0}
	{
	}

	Font::Font(std::string_view filepath, const int size)
	    : m_height {0}
	{
		if (!load(filepath, size))
		{
			PL_LOG(PL_ERROR, "Failed to load font: {0}.", filepath);
		}
	}

	Font::Font(Font&& f)
	{
		this->m_height     = f.m_height;
		this->m_characters = std::move(f.m_characters);
		f.m_characters.clear();
	}

	Font& Font::operator=(Font&& f)
	{
		if (this != &f)
		{
			this->m_height     = f.m_height;
			this->m_characters = std::move(f.m_characters);
			f.m_characters.clear();
		}

		return *this;
	}

	Font::~Font()
	{
		m_characters.clear();
	}

	bool Font::load(std::string_view filepath, const int size)
	{
		auto path    = std::filesystem::path {filepath};
		bool success = true;

		FT_Face face;
		if (FT_New_Face(FTLIB.lib(), path.string().c_str(), 0, &face) != 0)
		{
			PL_LOG(PL_ERROR, "Failed to load {0}.", filepath);
			success = false;
		}
		else
		{
			FT_Set_Pixel_Sizes(face, 0, size);

			int alignment = 0;
			glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			FT_UInt index = 0;
			auto c        = FT_Get_First_Char(face, &index);
			while (index)
			{
				FT_Load_Char(face, c, FT_LOAD_RENDER);

				Character c_obj;
				glBindTexture(GL_TEXTURE_2D, c_obj.m_gl_texture);
				glTexImage2D(
				    GL_TEXTURE_2D,
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
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				c_obj.m_size.x    = face->glyph->bitmap.width;
				c_obj.m_size.y    = face->glyph->bitmap.rows;
				c_obj.m_bearing.x = face->glyph->bitmap_left;
				c_obj.m_bearing.y = face->glyph->bitmap_top;
				c_obj.m_advance   = face->glyph->advance.x;

				m_characters.emplace(c, std::move(c_obj));

				c = FT_Get_Next_Char(face, c, &index);
			}

			glBindTexture(GL_TEXTURE_2D, 0);
			m_height = m_characters['X'].m_bearing.y;
			glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
		}

		FT_Done_Face(face);
		return success;
	}

	Character* Font::get_char(char c)
	{
		if (!m_characters.contains(c))
		{
			PL_LOG(PL_WARNING, "FontMap does not contain character: '{0}'.", c);
			return nullptr;
		}
		else
		{
			return &m_characters[c];
		}
	}

	const int Font::get_width(std::string_view text)
	{
		int width = 0;

		for (const char c : text)
		{
			width += (m_characters[c].m_advance >> 6);
		}

		return width;
	}

	const int Font::get_height() const
	{
		return m_height;
	}
} // namespace qs