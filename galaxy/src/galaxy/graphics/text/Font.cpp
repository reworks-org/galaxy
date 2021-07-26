///
/// Font.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/Config.hpp"
#include "galaxy/fs/FileSystem.hpp"
#include "galaxy/graphics/text/FreeType.hpp"

#include "Font.hpp"

namespace galaxy
{
	namespace graphics
	{
		Font::Font() noexcept
			: m_height {0}
			, m_size {0}
			, m_filename {""}
		{
		}

		Font::Font(std::string_view filepath, const int size)
		{
			if (!create(filepath, size))
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load font file: {0}.", filepath);
			}
		}

		Font::Font(Font&& f) noexcept
		{
			this->m_height     = f.m_height;
			this->m_size       = f.m_size;
			this->m_filename   = std::move(f.m_filename);
			this->m_characters = std::move(f.m_characters);
		}

		Font& Font::operator=(Font&& f) noexcept
		{
			if (this != &f)
			{
				this->m_height     = f.m_height;
				this->m_size       = f.m_size;
				this->m_filename   = std::move(f.m_filename);
				this->m_characters = std::move(f.m_characters);
			}

			return *this;
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

					FT_Face ft_face;
					if (FT_New_Face(FT_HANDLE.lib(), path.value().c_str(), 0, &ft_face) != FT_OK)
					{
						GALAXY_LOG(GALAXY_ERROR, "Failed to create font face for: {0}.", file);
						success = false;
					}
					else
					{
						FT_Set_Pixel_Sizes(ft_face, 0, m_size);
						m_height = ((ft_face->size->metrics.ascender - ft_face->size->metrics.descender) >> 6);

						int original_alignment = 0;
						glGetIntegerv(GL_UNPACK_ALIGNMENT, &original_alignment);
						glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

						FT_UInt ft_index = 0;
						FT_ULong ft_char = FT_Get_First_Char(ft_face, &ft_index);
						while (ft_index)
						{
							if (FT_Load_Char(ft_face, ft_char, FT_LOAD_RENDER) != FT_OK)
							{
								GALAXY_LOG(GALAXY_WARNING, "Failed to load glyph: '{0}', skipping.", ft_char);
							}
							else
							{
								Character character;
								glBindTexture(GL_TEXTURE_2D, character.m_texture);
								glTexImage2D(GL_TEXTURE_2D,
											 0,
											 GL_RED,
											 ft_face->glyph->bitmap.width,
											 ft_face->glyph->bitmap.rows,
											 0,
											 GL_RED,
											 GL_UNSIGNED_BYTE,
											 ft_face->glyph->bitmap.buffer);
								glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
								glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
								glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
								glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
								glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, SL_HANDLE.config()->get<float>("ansio-filter"));

								character.m_size.x    = ft_face->glyph->bitmap.width;
								character.m_size.y    = ft_face->glyph->bitmap.rows;
								character.m_bearing.x = ft_face->glyph->bitmap_left;
								character.m_bearing.y = ft_face->glyph->bitmap_top;
								character.m_advance   = ft_face->glyph->advance.x;

								m_characters[static_cast<char>(ft_char)] = std::move(character);
							}

							ft_char = FT_Get_Next_Char(ft_face, ft_char, &ft_index);
						}

						glBindTexture(GL_TEXTURE_2D, 0);
						glPixelStorei(GL_UNPACK_ALIGNMENT, original_alignment);
						FT_Done_Face(ft_face);
					}
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

		const int Font::get_width(std::string_view text) noexcept
		{
			return std::accumulate(text.begin(),
								   text.end(),
								   0,
								   [this](int width, const char c)
								   {
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