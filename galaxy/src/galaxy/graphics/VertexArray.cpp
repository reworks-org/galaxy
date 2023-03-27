///
/// VertexArrary.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/graphics/AttributeBindings.hpp"
#include "galaxy/graphics/BufferBindings.hpp"

#include "VertexArray.hpp"

namespace galaxy
{
	namespace graphics
	{
		VertexArray::VertexArray()
			: m_vao {0}
		{
			glCreateVertexArrays(1, &m_vao);
		}

		VertexArray::VertexArray(VertexArray&& va)
			: m_vao {0}
		{
			this->destroy();

			this->m_vao             = va.m_vao;
			this->m_vbo             = std::move(va.m_vbo);
			this->m_ibo             = std::move(va.m_ibo);
			this->m_instance_buffer = std::move(va.m_instance_buffer);

			va.m_vao = 0;
		}

		VertexArray& VertexArray::operator=(VertexArray&& va)
		{
			if (this != &va)
			{
				this->destroy();

				this->m_vao             = va.m_vao;
				this->m_vbo             = std::move(va.m_vbo);
				this->m_ibo             = std::move(va.m_ibo);
				this->m_instance_buffer = std::move(va.m_instance_buffer);

				va.m_vao = 0;
			}

			return *this;
		}

		VertexArray::~VertexArray()
		{
			destroy();
		}

		void VertexArray::create(std::span<Vertex> vertices,
			const StorageFlag vertices_flag,
			std::span<unsigned int> indices,
			const StorageFlag indices_flag)
		{
			m_vbo.create(vertices, vertices_flag);
			m_ibo.create(indices, indices_flag);

			// Bind vertex to 0 (vertex buffer bind point, different from attribute bind point).
			glVertexArrayVertexBuffer(m_vao, static_cast<unsigned int>(BufferBinding::VERTEX_BUFFER_POINT), m_vbo.id(), 0, sizeof(Vertex));
			glVertexArrayElementBuffer(m_vao, m_ibo.id());

			glEnableVertexArrayAttrib(m_vao, static_cast<unsigned int>(AttributeBinding::POSITION_POINT)); // Pos
			glEnableVertexArrayAttrib(m_vao, static_cast<unsigned int>(AttributeBinding::TEXEL_POINT));    // Texels
			glEnableVertexArrayAttrib(m_vao, static_cast<unsigned int>(AttributeBinding::COLOUR_POINT));   // Colour

			// bind point, size (i.e. vec2, vec3, etc) floats not unsigned ints, not normalized, offset in data structure.
			glVertexArrayAttribFormat(m_vao, static_cast<unsigned int>(AttributeBinding::POSITION_POINT), 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, m_pos));
			glVertexArrayAttribFormat(m_vao, static_cast<unsigned int>(AttributeBinding::TEXEL_POINT), 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, m_texels));
			glVertexArrayAttribFormat(m_vao, static_cast<unsigned int>(AttributeBinding::COLOUR_POINT), 4, GL_FLOAT, GL_FALSE, offsetof(Vertex, m_colour));

			// VAO, attribute bind point, vertex buffer bind point.
			glVertexArrayAttribBinding(m_vao,
				static_cast<unsigned int>(AttributeBinding::POSITION_POINT),
				static_cast<unsigned int>(BufferBinding::VERTEX_BUFFER_POINT));
			glVertexArrayAttribBinding(m_vao,
				static_cast<unsigned int>(AttributeBinding::TEXEL_POINT),
				static_cast<unsigned int>(BufferBinding::VERTEX_BUFFER_POINT));
			glVertexArrayAttribBinding(m_vao,
				static_cast<unsigned int>(AttributeBinding::COLOUR_POINT),
				static_cast<unsigned int>(BufferBinding::VERTEX_BUFFER_POINT));
		}

		void VertexArray::create(const unsigned int size, std::span<unsigned int> indices, const StorageFlag indices_flag)
		{
			m_vbo.reserve(size);
			m_ibo.create(indices, indices_flag);

			// Bind vertex to 0 (vertex buffer bind point, different from attribute bind point).
			glVertexArrayVertexBuffer(m_vao, static_cast<unsigned int>(BufferBinding::VERTEX_BUFFER_POINT), m_vbo.id(), 0, sizeof(Vertex));
			glVertexArrayElementBuffer(m_vao, m_ibo.id());

			glEnableVertexArrayAttrib(m_vao, static_cast<unsigned int>(AttributeBinding::POSITION_POINT)); // Pos
			glEnableVertexArrayAttrib(m_vao, static_cast<unsigned int>(AttributeBinding::TEXEL_POINT));    // Texels
			glEnableVertexArrayAttrib(m_vao, static_cast<unsigned int>(AttributeBinding::COLOUR_POINT));   // Colour

			// bind point, size (i.e. vec2, vec3, etc) floats not unsigned ints, not normalized, offset in data structure.
			glVertexArrayAttribFormat(m_vao, static_cast<unsigned int>(AttributeBinding::POSITION_POINT), 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, m_pos));
			glVertexArrayAttribFormat(m_vao, static_cast<unsigned int>(AttributeBinding::TEXEL_POINT), 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, m_texels));
			glVertexArrayAttribFormat(m_vao, static_cast<unsigned int>(AttributeBinding::COLOUR_POINT), 4, GL_FLOAT, GL_FALSE, offsetof(Vertex, m_colour));

			// VAO, attribute bind point, vertex buffer bind point.
			glVertexArrayAttribBinding(m_vao,
				static_cast<unsigned int>(AttributeBinding::POSITION_POINT),
				static_cast<unsigned int>(BufferBinding::VERTEX_BUFFER_POINT));
			glVertexArrayAttribBinding(m_vao,
				static_cast<unsigned int>(AttributeBinding::TEXEL_POINT),
				static_cast<unsigned int>(BufferBinding::VERTEX_BUFFER_POINT));
			glVertexArrayAttribBinding(m_vao,
				static_cast<unsigned int>(AttributeBinding::COLOUR_POINT),
				static_cast<unsigned int>(BufferBinding::VERTEX_BUFFER_POINT));
		}

		void VertexArray::set_instanced(std::span<glm::vec2> offsets)
		{
			m_instance_buffer.create(offsets);

			glVertexArrayVertexBuffer(m_vao, static_cast<unsigned int>(BufferBinding::INSTANCE_BUFFER_POINT), m_instance_buffer.id(), 0, sizeof(float) * 2);
			glEnableVertexArrayAttrib(m_vao, static_cast<unsigned int>(AttributeBinding::OFFSET_POINT));
			glVertexArrayAttribFormat(m_vao, static_cast<unsigned int>(AttributeBinding::OFFSET_POINT), 2, GL_FLOAT, GL_FALSE, 0);
			glVertexArrayAttribBinding(m_vao,
				static_cast<unsigned int>(AttributeBinding::OFFSET_POINT),
				static_cast<unsigned int>(BufferBinding::INSTANCE_BUFFER_POINT));
			glVertexArrayBindingDivisor(m_vao, static_cast<unsigned int>(BufferBinding::INSTANCE_BUFFER_POINT), 1);
		}

		void VertexArray::sub_buffer(const unsigned int index, std::span<Vertex> vertices)
		{
			m_vbo.sub_buffer(index, vertices);
		}

		void VertexArray::clear()
		{
			m_vbo.clear();
		}

		void VertexArray::destroy()
		{
			if (m_vao != 0)
			{
				glDeleteVertexArrays(1, &m_vao);
				m_vao = 0;
			}
		}

		void VertexArray::bind()
		{
			glBindVertexArray(m_vao);
		}

		void VertexArray::unbind()
		{
			glBindVertexArray(0);
		}

		int VertexArray::index_count() const
		{
			return m_ibo.index_count();
		}

		unsigned int VertexArray::id() const
		{
			return m_vao;
		}

		unsigned int VertexArray::vbo() const
		{
			return m_vbo.id();
		}

		unsigned int VertexArray::ibo() const
		{
			return m_ibo.id();
		}
	} // namespace graphics
} // namespace galaxy