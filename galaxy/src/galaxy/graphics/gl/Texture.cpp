///
/// Texture.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/graphics/gl/TextureView.hpp"
#include "galaxy/utils/Globals.hpp"

#include "Texture.hpp"

#ifdef GALAXY_WIN_PLATFORM
#pragma warning(push)
#pragma warning(disable : 26493)
#endif

namespace galaxy
{
	namespace graphics
	{
		Texture::Texture()
			: m_id {0}
			, m_width {0}
			, m_height {0}
			, m_mode {TextureMode::CLAMP_TO_EDGE}
			, m_filter {TextureFilter::NEAREST}
			, m_anisotropy {0}
		{
		}

		Texture::Texture(Texture&& t)
		{
			if (this->m_id != 0)
			{
				glDeleteTextures(1, &this->m_id);
			}

			this->m_id         = t.m_id;
			this->m_width      = t.m_width;
			this->m_height     = t.m_height;
			this->m_filter     = t.m_filter;
			this->m_mode       = t.m_mode;
			this->m_anisotropy = t.m_anisotropy;

			t.m_id = 0;
		}

		Texture& Texture::operator=(Texture&& t)
		{
			if (this != &t)
			{
				if (this->m_id != 0)
				{
					glDeleteTextures(1, &this->m_id);
				}

				this->m_id         = t.m_id;
				this->m_width      = t.m_width;
				this->m_height     = t.m_height;
				this->m_filter     = t.m_filter;
				this->m_mode       = t.m_mode;
				this->m_anisotropy = t.m_anisotropy;

				t.m_id = 0;
			}

			return *this;
		}

		Texture::~Texture()
		{
			if (m_id != 0)
			{
				glDeleteTextures(1, &m_id);
			}
		}

		TextureView Texture::make_view(const unsigned int minlevel, const unsigned int numlevels, const unsigned int minlayer, const unsigned int numlayers)
		{
			TextureView tv {m_id, minlevel, numlevels, minlayer, numlayers};
			tv.m_mode       = m_mode;
			tv.m_filter     = m_filter;
			tv.m_anisotropy = m_anisotropy;
			tv.m_width      = m_width;
			tv.m_height     = m_height;

			return tv;
		}

		void Texture::mode(const TextureMode mode)
		{
			glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, static_cast<GLint>(mode));
			glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, static_cast<GLint>(mode));
		}

		TextureMode Texture::mode() const
		{
			return m_mode;
		}

		void Texture::filter(const TextureFilter filter)
		{
			m_filter = filter;

			switch (m_filter)
			{
				case TextureFilter::NEAREST:
					glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
					glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					break;

				case TextureFilter::TRILINEAR:
					glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
					glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					break;
			}
		}

		TextureFilter Texture::filter() const
		{
			return m_filter;
		}

		void Texture::anisotropy(const int level)
		{
			m_anisotropy = std::clamp(level, 0, 16);
			if (m_anisotropy <= 0)
			{
				m_anisotropy = 0;
			}
			else if (m_anisotropy <= 2)
			{
				m_anisotropy = 2;
			}
			else if (m_anisotropy <= 4)
			{
				m_anisotropy = 4;
			}
			else if (m_anisotropy <= 8)
			{
				m_anisotropy = 8;
			}
			else
			{
				m_anisotropy = 16;
			}

			glTextureParameterf(m_id, GL_TEXTURE_MAX_ANISOTROPY, static_cast<float>(m_anisotropy));
		}

		int Texture::anisotropy() const
		{
			return m_anisotropy;
		}

		float Texture::width() const
		{
			return static_cast<float>(m_width);
		}

		float Texture::height() const
		{
			return static_cast<float>(m_height);
		}

		unsigned int Texture::id() const
		{
			return m_id;
		}
	} // namespace graphics
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
#pragma warning(pop)
#endif
