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
			// Only update opacity and texels.
			// Position is done GPU side.

			qs::Vertex* vertex = nullptr;
			glm::vec3* texels = &getTexelTransform();
			
			vertex = &m_transformedVertexs[0];
			vertex->m_texels[0] = texels->x;
			vertex->m_texels[1] = texels->y;
			vertex->m_colour[3] = texels->z;

			vertex = &m_transformedVertexs[1];
			vertex->m_texels[0] = texels->x + m_width;
			vertex->m_texels[1] = texels->y;
			vertex->m_colour[3] = texels->z;

			vertex = &m_transformedVertexs[2];
			vertex->m_texels[0] = texels->x + m_width;
			vertex->m_texels[1] = texels->y + m_height;
			vertex->m_colour[3] = texels->z;

			vertex = &m_transformedVertexs[3];
			vertex->m_texels[0] = texels->x;
			vertex->m_texels[1] = texels->y + m_height;
			vertex->m_colour[3] = texels->z;

			glNamedBufferSubData(m_vertexBuffer.getID(), 0, m_transformedVertexs.size() * sizeof(qs::Vertex), m_transformedVertexs.data());
			setDirty(false);
		}
	}
}