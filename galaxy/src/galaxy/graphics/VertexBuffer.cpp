///
/// VertexBuffer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/error/Log.hpp"

#include "VertexBuffer.hpp"

namespace galaxy
{
	namespace graphics
	{
		VertexBuffer::VertexBuffer()
			: m_vbo {0}
			, m_size {0}
		{
			glCreateBuffers(1, &m_vbo);
		}

		VertexBuffer::VertexBuffer(VertexBuffer&& vb)
		{
			this->destroy();

			this->m_vbo  = vb.m_vbo;
			this->m_size = vb.m_size;

			vb.m_vbo = 0;
		}

		VertexBuffer& VertexBuffer::operator=(VertexBuffer&& vb)
		{
			if (this != &vb)
			{
				this->destroy();

				this->m_vbo  = vb.m_vbo;
				this->m_size = vb.m_size;

				vb.m_vbo = 0;
			}

			return *this;
		}

		VertexBuffer::~VertexBuffer()
		{
			destroy();
		}

		void VertexBuffer::create(std::span<Vertex> vertices, const StorageFlag flag)
		{
			if (!vertices.empty())
			{
				m_size = static_cast<unsigned int>(vertices.size());
				glNamedBufferData(m_vbo, vertices.size_bytes(), vertices.data(), static_cast<GLenum>(flag));
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Attempted to upload empty vertex buffer.");
			}
		}

		void VertexBuffer::reserve(const unsigned int size)
		{
			if (size > 0u)
			{
				m_size = size;
				glNamedBufferData(m_vbo, m_size * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Reserved vertex buffer size must be greater than 0.");
			}
		}

		void VertexBuffer::sub_buffer(const unsigned int index, std::span<Vertex> vertices)
		{
			if (!vertices.empty() && (vertices.size() <= (m_size - (index * 4))))
			{
				glNamedBufferSubData(m_vbo, index * sizeof(Vertex), vertices.size_bytes(), vertices.data());
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Attempted to buffer outside vertex buffer range.");
			}
		}

		void VertexBuffer::clear()
		{
			glInvalidateBufferData(m_vbo);
			glNamedBufferData(m_vbo, m_size * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		}

		void VertexBuffer::destroy()
		{
			if (m_vbo != 0)
			{
				glDeleteBuffers(1, &m_vbo);
				m_vbo = 0;
			}
		}

		unsigned int VertexBuffer::id() const
		{
			return m_vbo;
		}
	} // namespace graphics
} // namespace galaxy