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

		// Bind vertex to 0 (vertex buffer bind point, different from attribute bind point).
		// Then we bind the vbo as our ebo, since we are using a shared buffer.
		glVertexArrayVertexBuffer(m_id, static_cast<unsigned int>(GLBufferLocation::VERTEX_BUFFER_POINT), m_vbo.id(), 0, sizeof(Vertex));
		glVertexArrayElementBuffer(m_id, m_vbo.id());

		// Enable each binding point for the vertex attributes.
		glEnableVertexArrayAttrib(m_id, static_cast<unsigned int>(GLAttributeBinding::POSITION_POINT));
		glEnableVertexArrayAttrib(m_id, static_cast<unsigned int>(GLAttributeBinding::TEXEL_POINT));
		glEnableVertexArrayAttrib(m_id, static_cast<unsigned int>(GLAttributeBinding::HANDLE_POINT));

		// Specify each binding point for the vertex attributes.
		// size (i.e. vec2, vec3, etc) floats not unsigned ints, not normalized, offset in data structure.
		glVertexArrayAttribFormat(m_id, static_cast<unsigned int>(GLAttributeBinding::POSITION_POINT), 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, m_pos));
		glVertexArrayAttribFormat(m_id, static_cast<unsigned int>(GLAttributeBinding::TEXEL_POINT), 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, m_texels));
		glVertexArrayAttribLFormat(m_id, static_cast<unsigned int>(GLAttributeBinding::HANDLE_POINT), 1, GL_UNSIGNED_INT64_ARB, offsetof(Vertex, m_handle));

		// VAO, attribute bind point, vertex buffer bind point.
		glVertexArrayAttribBinding(m_id, static_cast<unsigned int>(GLAttributeBinding::POSITION_POINT), static_cast<unsigned int>(GLBufferLocation::VERTEX_BUFFER_POINT));
		glVertexArrayAttribBinding(m_id, static_cast<unsigned int>(GLAttributeBinding::TEXEL_POINT), static_cast<unsigned int>(GLBufferLocation::VERTEX_BUFFER_POINT));
		glVertexArrayAttribBinding(m_id, static_cast<unsigned int>(GLAttributeBinding::HANDLE_POINT), static_cast<unsigned int>(GLBufferLocation::VERTEX_BUFFER_POINT));
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
		}
	}

	void VertexArray::buffer(std::span<Vertex> vertices, std::span<unsigned int> indices)
	{
		m_vbo.buffer(vertices, indices);
	}

	void VertexArray::reserve(const int vertex_count, const int index_count)
	{
		m_vbo.reserve(vertex_count, index_count);
	}

	void
	VertexArray::sub_buffer(const unsigned int vi, const int vertex_size, const std::span<Vertex> vertices, unsigned int ei, const int index_size, std::span<unsigned int> indices)
	{
		m_vbo.sub_buffer(vi, vertex_size, vertices, ei, index_size, indices);
	}

	void VertexArray::erase(const unsigned int vi, const int vertex_count, const unsigned int ei, const int index_count)
	{
		m_vbo.erase(vi, vertex_count, ei, index_count);
	}

	void VertexArray::clear()
	{
		m_vbo.clear();
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
