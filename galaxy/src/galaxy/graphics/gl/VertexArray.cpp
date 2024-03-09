///
/// VertexArrary.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glm/mat4x4.hpp>

#include "galaxy/graphics/gl/Enums.hpp"

#include "VertexArray.hpp"

namespace galaxy
{
	namespace graphics
	{
		VertexArray::VertexArray()
			: m_id {0}
			, m_instances {1}
		{
			glCreateVertexArrays(1, &m_id);
		}

		VertexArray::VertexArray(VertexArray&& v)
		{
			if (this->m_id != 0)
			{
				glDeleteVertexArrays(1, &this->m_id);
			}

			this->m_id        = v.m_id;
			this->m_vbo       = std::move(v.m_vbo);
			this->m_instances = v.m_instances;

			v.m_id = 0;
		}

		VertexArray& VertexArray::operator=(VertexArray&& v)
		{
			if (this != &v)
			{
				if (this->m_id != 0)
				{
					glDeleteVertexArrays(1, &this->m_id);
				}

				this->m_id        = v.m_id;
				this->m_vbo       = std::move(v.m_vbo);
				this->m_instances = v.m_instances;

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

		void VertexArray::buffer(std::span<Vertex> vertices, std::span<unsigned int> indicies)
		{
			m_vbo.buffer(vertices, indicies);

			// Bind vertex to 0 (vertex buffer bind point, different from attribute bind point).
			glVertexArrayVertexBuffer(m_id, static_cast<unsigned int>(BufferBinding::VERTEX_BUFFER_POINT), m_vbo.id(), 0, sizeof(Vertex));
			glVertexArrayElementBuffer(m_id, m_vbo.id());

			glEnableVertexArrayAttrib(m_id, static_cast<unsigned int>(AttributeBinding::POSITION_POINT)); // Pos
			glEnableVertexArrayAttrib(m_id, static_cast<unsigned int>(AttributeBinding::TEXEL_POINT));    // Texels

			// bind point, size (i.e. vec2, vec3, etc) floats not unsigned ints, not normalized, offset in data structure.
			glVertexArrayAttribFormat(m_id, static_cast<unsigned int>(AttributeBinding::POSITION_POINT), 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, m_pos));
			glVertexArrayAttribFormat(m_id, static_cast<unsigned int>(AttributeBinding::TEXEL_POINT), 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, m_texels));

			// VAO, attribute bind point, vertex buffer bind point.
			glVertexArrayAttribBinding(m_id,
				static_cast<unsigned int>(AttributeBinding::POSITION_POINT),
				static_cast<unsigned int>(BufferBinding::VERTEX_BUFFER_POINT));
			glVertexArrayAttribBinding(m_id,
				static_cast<unsigned int>(AttributeBinding::TEXEL_POINT),
				static_cast<unsigned int>(BufferBinding::VERTEX_BUFFER_POINT));
		}

		void VertexArray::buffer(const int vertex_count, std::span<unsigned int> indicies)
		{
			m_vbo.buffer(vertex_count, indicies);

			// Bind vertex to 0 (, different from attribute bind point).
			glVertexArrayVertexBuffer(m_id, static_cast<unsigned int>(BufferBinding::VERTEX_BUFFER_POINT), m_vbo.id(), 0, sizeof(Vertex));
			glVertexArrayElementBuffer(m_id, m_vbo.id());

			glEnableVertexArrayAttrib(m_id, static_cast<unsigned int>(AttributeBinding::POSITION_POINT)); // Pos
			glEnableVertexArrayAttrib(m_id, static_cast<unsigned int>(AttributeBinding::TEXEL_POINT));    // Texels

			// bind point, size (i.e. vec2, vec3, etc) floats not unsigned ints, not normalized, offset in data structure.
			glVertexArrayAttribFormat(m_id, static_cast<unsigned int>(AttributeBinding::POSITION_POINT), 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, m_pos));
			glVertexArrayAttribFormat(m_id, static_cast<unsigned int>(AttributeBinding::TEXEL_POINT), 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, m_texels));

			// VAO, attribute bind point, vertex buffer bind point.
			glVertexArrayAttribBinding(m_id,
				static_cast<unsigned int>(AttributeBinding::POSITION_POINT),
				static_cast<unsigned int>(BufferBinding::VERTEX_BUFFER_POINT));
			glVertexArrayAttribBinding(m_id,
				static_cast<unsigned int>(AttributeBinding::TEXEL_POINT),
				static_cast<unsigned int>(BufferBinding::VERTEX_BUFFER_POINT));
		}

		void VertexArray::sub_buffer(const unsigned int index, std::span<Vertex> vertices)
		{
			m_vbo.sub_buffer(index, vertices);
		}

		void VertexArray::set_instanced(const InstanceBuffer& ib)
		{
			m_instances = ib.amount();

			glVertexArrayVertexBuffer(m_id, static_cast<unsigned int>(BufferBinding::INSTANCE_BUFFER_POINT), ib.id(), 0, sizeof(glm::mat4));

			glEnableVertexArrayAttrib(m_id, static_cast<unsigned int>(AttributeBinding::OFFSET_POINT));
			glEnableVertexArrayAttrib(m_id, static_cast<unsigned int>(AttributeBinding::OFFSET_POINT) + 1u);
			glEnableVertexArrayAttrib(m_id, static_cast<unsigned int>(AttributeBinding::OFFSET_POINT) + 2u);
			glEnableVertexArrayAttrib(m_id, static_cast<unsigned int>(AttributeBinding::OFFSET_POINT) + 3u);

			glVertexArrayAttribFormat(m_id, static_cast<unsigned int>(AttributeBinding::OFFSET_POINT), 4, GL_FLOAT, GL_FALSE, 0);
			glVertexArrayAttribFormat(m_id, static_cast<unsigned int>(AttributeBinding::OFFSET_POINT) + 1u, 4, GL_FLOAT, GL_FALSE, (sizeof(glm::vec4)));
			glVertexArrayAttribFormat(m_id, static_cast<unsigned int>(AttributeBinding::OFFSET_POINT) + 2u, 4, GL_FLOAT, GL_FALSE, (2 * sizeof(glm::vec4)));
			glVertexArrayAttribFormat(m_id, static_cast<unsigned int>(AttributeBinding::OFFSET_POINT) + 3u, 4, GL_FLOAT, GL_FALSE, (3 * sizeof(glm::vec4)));

			glVertexArrayAttribBinding(m_id,
				static_cast<unsigned int>(AttributeBinding::OFFSET_POINT),
				static_cast<unsigned int>(BufferBinding::INSTANCE_BUFFER_POINT));
			glVertexArrayAttribBinding(m_id,
				static_cast<unsigned int>(AttributeBinding::OFFSET_POINT) + 1u,
				static_cast<unsigned int>(BufferBinding::INSTANCE_BUFFER_POINT));
			glVertexArrayAttribBinding(m_id,
				static_cast<unsigned int>(AttributeBinding::OFFSET_POINT) + 2u,
				static_cast<unsigned int>(BufferBinding::INSTANCE_BUFFER_POINT));
			glVertexArrayAttribBinding(m_id,
				static_cast<unsigned int>(AttributeBinding::OFFSET_POINT) + 3u,
				static_cast<unsigned int>(BufferBinding::INSTANCE_BUFFER_POINT));

			glVertexArrayBindingDivisor(m_id, static_cast<unsigned int>(AttributeBinding::OFFSET_POINT), 1);
			glVertexArrayBindingDivisor(m_id, static_cast<unsigned int>(AttributeBinding::OFFSET_POINT) + 1u, 1);
			glVertexArrayBindingDivisor(m_id, static_cast<unsigned int>(AttributeBinding::OFFSET_POINT) + 2u, 1);
			glVertexArrayBindingDivisor(m_id, static_cast<unsigned int>(AttributeBinding::OFFSET_POINT) + 3u, 1);
		}

		void VertexArray::bind()
		{
			glBindVertexArray(m_id);
		}

		void VertexArray::unbind()
		{
			glBindVertexArray(0);
		}

		int VertexArray::count() const
		{
			return m_vbo.count();
		}

		void* VertexArray::offset()
		{
			return m_vbo.offset();
		}

		int VertexArray::instances() const
		{
			return m_instances;
		}

		VertexBuffer& VertexArray::vbo()
		{
			return m_vbo;
		}

		unsigned int VertexArray::id() const
		{
			return m_id;
		}
	} // namespace graphics
} // namespace galaxy
