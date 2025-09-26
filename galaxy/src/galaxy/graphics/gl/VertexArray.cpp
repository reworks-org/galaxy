///
/// VertexArrary.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glm/mat4x4.hpp>

#include "galaxy/graphics/gl/GLEnums.hpp"
#include "galaxy/graphics/Enums.hpp"

#include "VertexArray.hpp"

namespace galaxy
{
	VertexArray::VertexArray()
		: m_id {0}
	{
		glCreateVertexArrays(1, &m_id);
	}

	VertexArray::VertexArray(VertexArray&& v) noexcept
	{
		if (this->m_id != 0)
		{
			glDeleteVertexArrays(1, &this->m_id);
		}

		this->m_id  = v.m_id;
		this->m_vbo = std::move(v.m_vbo);

		v.m_id = 0;
	}

	VertexArray& VertexArray::operator=(VertexArray&& v) noexcept
	{
		if (this != &v)
		{
			if (this->m_id != 0)
			{
				glDeleteVertexArrays(1, &this->m_id);
			}

			this->m_id  = v.m_id;
			this->m_vbo = std::move(v.m_vbo);

			v.m_id = 0;
		}

		return *this;
	}

	VertexArray::~VertexArray()
	{
		if (m_id != 0)
		{
			glDeleteVertexArrays(1, &m_id);
			m_id = 0;
		}
	}

	void VertexArray::buffer(std::span<Vertex> vertices, std::span<unsigned int> indicies)
	{
		m_vbo.buffer(vertices, indicies);

		// Bind vertex to 0 (vertex buffer bind point, different from attribute bind point).
		glVertexArrayVertexBuffer(m_id, static_cast<unsigned int>(GLBufferBinding::VERTEX_BUFFER_POINT), m_vbo.id(), 0, sizeof(Vertex));
		glVertexArrayElementBuffer(m_id, m_vbo.id());

		glEnableVertexArrayAttrib(m_id, static_cast<unsigned int>(GLAttributeBinding::POSITION_POINT)); // Pos
		glEnableVertexArrayAttrib(m_id, static_cast<unsigned int>(GLAttributeBinding::TEXEL_POINT));    // Texels

		// bind point, size (i.e. vec2, vec3, etc) floats not unsigned ints, not normalized, offset in data structure.
		glVertexArrayAttribFormat(m_id, static_cast<unsigned int>(GLAttributeBinding::POSITION_POINT), 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, m_pos));
		glVertexArrayAttribFormat(m_id, static_cast<unsigned int>(GLAttributeBinding::TEXEL_POINT), 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, m_texels));

		// VAO, attribute bind point, vertex buffer bind point.
		glVertexArrayAttribBinding(m_id, static_cast<unsigned int>(GLAttributeBinding::POSITION_POINT), static_cast<unsigned int>(GLBufferBinding::VERTEX_BUFFER_POINT));
		glVertexArrayAttribBinding(m_id, static_cast<unsigned int>(GLAttributeBinding::TEXEL_POINT), static_cast<unsigned int>(GLBufferBinding::VERTEX_BUFFER_POINT));
	}

	void VertexArray::buffer(const int vertex_count, std::span<unsigned int> indicies)
	{
		m_vbo.buffer(vertex_count, indicies);

		// Bind vertex to 0 (, different from attribute bind point).
		glVertexArrayVertexBuffer(m_id, static_cast<unsigned int>(GLBufferBinding::VERTEX_BUFFER_POINT), m_vbo.id(), 0, sizeof(Vertex));
		glVertexArrayElementBuffer(m_id, m_vbo.id());

		glEnableVertexArrayAttrib(m_id, static_cast<unsigned int>(GLAttributeBinding::POSITION_POINT)); // Pos
		glEnableVertexArrayAttrib(m_id, static_cast<unsigned int>(GLAttributeBinding::TEXEL_POINT));    // Texels

		// bind point, size (i.e. vec2, vec3, etc) floats not unsigned ints, not normalized, offset in data structure.
		glVertexArrayAttribFormat(m_id, static_cast<unsigned int>(GLAttributeBinding::POSITION_POINT), 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, m_pos));
		glVertexArrayAttribFormat(m_id, static_cast<unsigned int>(GLAttributeBinding::TEXEL_POINT), 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, m_texels));

		// VAO, attribute bind point, vertex buffer bind point.
		glVertexArrayAttribBinding(m_id, static_cast<unsigned int>(GLAttributeBinding::POSITION_POINT), static_cast<unsigned int>(GLBufferBinding::VERTEX_BUFFER_POINT));
		glVertexArrayAttribBinding(m_id, static_cast<unsigned int>(GLAttributeBinding::TEXEL_POINT), static_cast<unsigned int>(GLBufferBinding::VERTEX_BUFFER_POINT));
	}

	void VertexArray::sub_buffer(const unsigned int index, std::span<Vertex> vertices)
	{
		m_vbo.sub_buffer(index, vertices);
	}

	void VertexArray::bind() const
	{
		glBindVertexArray(m_id);
	}

	void VertexArray::unbind() const
	{
		glBindVertexArray(0);
	}

	int VertexArray::count() const noexcept
	{
		return m_vbo.count();
	}

	void* VertexArray::offset() noexcept
	{
		return m_vbo.offset();
	}

	VertexBuffer& VertexArray::vbo() noexcept
	{
		return m_vbo;
	}

	unsigned int VertexArray::id() const noexcept
	{
		return m_id;
	}
} // namespace galaxy
