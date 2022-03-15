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
		VertexArray::VertexArray() noexcept
			: m_vao {0}
			, m_vbo {nullptr}
			, m_ibo {nullptr}
			, m_instance_buffer {nullptr}
		{
			glCreateVertexArrays(1, &m_vao);
		}

		VertexArray::VertexArray(VertexArray&& va) noexcept
		{
			this->m_vao             = va.m_vao;
			this->m_vbo             = std::move(va.m_vbo);
			this->m_ibo             = std::move(va.m_ibo);
			this->m_instance_buffer = std::move(va.m_instance_buffer);

			va.m_vao = 0;
		}

		VertexArray& VertexArray::operator=(VertexArray&& va) noexcept
		{
			if (this != &va)
			{
				this->m_vao             = va.m_vao;
				this->m_vbo             = std::move(va.m_vbo);
				this->m_ibo             = std::move(va.m_ibo);
				this->m_instance_buffer = std::move(va.m_instance_buffer);

				va.m_vao = 0;
			}

			return *this;
		}

		VertexArray::~VertexArray() noexcept
		{
			glDeleteVertexArrays(1, &m_vao);
		}

		void VertexArray::create(std::unique_ptr<VertexBuffer> vb, std::unique_ptr<IndexBuffer> ib) noexcept
		{
			m_vbo.reset();
			m_ibo.reset();

			m_vbo = std::move(vb);
			m_ibo = std::move(ib);

			// Bind vertex to 0 (vertex buffer bind point, different from attribute bind point).
			glVertexArrayVertexBuffer(m_vao, static_cast<unsigned int>(BufferBinding::VERTEX_BUFFER_POINT), m_vbo->id(), 0, sizeof(Vertex));
			glVertexArrayElementBuffer(m_vao, m_ibo->id());

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

		void VertexArray::set_instanced(std::unique_ptr<InstanceBuffer> instancing) noexcept
		{
			m_instance_buffer.reset();
			m_instance_buffer = std::move(instancing);

			glVertexArrayVertexBuffer(m_vao, static_cast<unsigned int>(BufferBinding::INSTANCE_BUFFER_POINT), m_instance_buffer->id(), 0, sizeof(float) * 2);
			glEnableVertexArrayAttrib(m_vao, static_cast<unsigned int>(AttributeBinding::OFFSET_POINT));
			glVertexArrayAttribFormat(m_vao, static_cast<unsigned int>(AttributeBinding::OFFSET_POINT), 2, GL_FLOAT, GL_FALSE, 0);
			glVertexArrayAttribBinding(m_vao,
				static_cast<unsigned int>(AttributeBinding::OFFSET_POINT),
				static_cast<unsigned int>(BufferBinding::INSTANCE_BUFFER_POINT));
			glVertexArrayBindingDivisor(m_vao, static_cast<unsigned int>(BufferBinding::INSTANCE_BUFFER_POINT), 1);
		}

		void VertexArray::bind() noexcept
		{
			glBindVertexArray(m_vao);
		}

		void VertexArray::unbind() noexcept
		{
			glBindVertexArray(0);
		}

		int VertexArray::index_count() const noexcept
		{
			return m_ibo->index_count();
		}

		unsigned int VertexArray::id() const noexcept
		{
			return m_vao;
		}

		unsigned int VertexArray::vbo() const noexcept
		{
			return m_vbo->id();
		}

		unsigned int VertexArray::ibo() const noexcept
		{
			return m_ibo->id();
		}
	} // namespace graphics
} // namespace galaxy