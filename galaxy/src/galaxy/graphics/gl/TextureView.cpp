///
/// TextureView.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>

#include "TextureView.hpp"

namespace galaxy
{
	TextureView::TextureView(
		const unsigned int parent,
		const unsigned int minlevel,
		const unsigned int numlevels,
		const unsigned int minlayer,
		const unsigned int numlayers
	) noexcept
	{
		glGenTextures(1, &m_id);
		glTextureView(m_id, GL_TEXTURE_2D, parent, GL_RGBA8, minlevel, numlevels, minlayer, numlayers);
	}

	TextureView::TextureView(TextureView&& t) noexcept
	{
		glDeleteTextures(1, &m_id);

		this->m_id = t.m_id;
		t.m_id     = 0;
	}

	TextureView& TextureView::operator=(TextureView&& t) noexcept
	{
		if (this != &t)
		{
			glDeleteTextures(1, &m_id);

			this->m_id = t.m_id;
			t.m_id     = 0;
		}

		return *this;
	}

	TextureView::~TextureView() noexcept
	{
		glDeleteTextures(1, &m_id);
	}

	unsigned int TextureView::id() const noexcept
	{
		return m_id;
	}
} // namespace galaxy
