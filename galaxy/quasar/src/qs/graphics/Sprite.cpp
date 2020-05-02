///
/// Sprite.cpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include "Sprite.hpp"

///
///
///
namespace qs
{
	Sprite::Sprite() noexcept
		:Renderable(), Transform()
	{
		m_isDirty = true;
	}

	Sprite::~Sprite()
	{
		unbind();
		glDeleteTextures(1, &m_textureHandle);
	}

	void Sprite::update() noexcept
	{
		if (isDirty())
		{
			// Set initial texture transform.
			static bool doOnce = true;
			if (doOnce)
			{
				doOnce = false;
				qs::VertexStorage* buff = &m_vertexBuffer.getVertexs();
				auto* vertex = &buff->at(0);
				// top left.
				setTexels(vertex->m_texels[0], vertex->m_texels[1]);
			}

			// Only update opacity and texels.
			// Position is done GPU side.
			qs::Vertex* vertex = nullptr;
			qs::VertexStorage* buff = &m_vertexBuffer.getVertexs();
			glm::vec3* texels = &getTexelTransform();
			
			vertex = &buff->at(0);
			vertex->m_texels[0] = texels->x;
			vertex->m_texels[1] = texels->y;
			vertex->m_colour[3] = texels->z;

			vertex = &buff->at(1);
			vertex->m_texels[0] = texels->x + m_width;
			vertex->m_texels[1] = texels->y;
			vertex->m_colour[3] = texels->z;

			vertex = &buff->at(2);
			vertex->m_texels[0] = texels->x + m_width;
			vertex->m_texels[1] = texels->y + m_height;
			vertex->m_colour[3] = texels->z;

			vertex = &buff->at(3);
			vertex->m_texels[0] = texels->x;
			vertex->m_texels[1] = texels->y + m_height;
			vertex->m_colour[3] = texels->z;

			glNamedBufferSubData(m_vertexBuffer.getID(), 0, buff->size() * sizeof(qs::Vertex), buff->data());
			setDirty(false);
		}
	}
}