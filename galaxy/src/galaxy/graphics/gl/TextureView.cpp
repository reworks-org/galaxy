///
/// TextureView.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>

#include "galaxy/utils/Globals.hpp"

#include "TextureView.hpp"

#ifdef GALAXY_WIN_PLATFORM
#pragma warning(push)
#pragma warning(disable : 26493)
#endif

namespace galaxy
{
	namespace graphics
	{
		TextureView::TextureView(const unsigned int parent,
			const unsigned int                      minlevel,
			const unsigned int                      numlevels,
			const unsigned int                      minlayer,
			const unsigned int                      numlayers)
			: Texture {}
		{
			glGenTextures(1, &m_id);
			glTextureView(m_id, GL_TEXTURE_2D, parent, GL_RGBA8, minlevel, numlevels, minlayer, numlayers);
		}

		TextureView::TextureView(TextureView&& t)
			: Texture {std::move(t)}
		{
		}

		TextureView& TextureView::operator=(TextureView&& t)
		{
			if (this != &t)
			{
				Texture::operator=(std::move(t));
			}

			return *this;
		}

		TextureView::~TextureView()
		{
			// We don't need to destroy the view here.
		}

		void TextureView::bind()
		{
			glBindTexture(GL_TEXTURE_2D, m_id);
		}

		void TextureView::unbind()
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	} // namespace graphics
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
#pragma warning(pop)
#endif
