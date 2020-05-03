///
/// Image.cpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#include "Image.hpp"

///
/// Core namespace.
///
namespace celestial
{
	Image::Image() noexcept
		:Widget()
	{
	}

	Image::~Image() noexcept
	{
	}

	void Image::make(const float x, const float y, const std::string& region) noexcept
	{
		auto* atlas = m_theme->getAtlas();
		auto* src = &atlas->getTexQuad(region);

		load(atlas->getTexture().getGLTexture(), atlas->getTexture().getWidth(), atlas->getTexture().getHeight());
		create<qs::BufferTypeDynamic>(*src);

		setRotationOrigin(src->m_width / 2.0f, src->m_height / 2.0f);
		setTexels(src->m_x, src->m_y);
		setOpacity(1.0f);
		move(x, y);
		setDirty(true);
	}

	void Image::update(protostar::ProtectedDouble* dt) noexcept
	{
		if (isDirty())
		{
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

			// buffer set in render() on main thread.
		}
	}

	void Image::render() noexcept
	{
		if (isDirty())
		{
			qs::VertexStorage* buff = &m_vertexBuffer.getVertexs();

			glNamedBufferSubData(m_vertexBuffer.getID(), 0, buff->size() * sizeof(qs::Vertex), buff->data());
			setDirty(false);
		}
	}
}