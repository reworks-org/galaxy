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
		VertexBuffer::VertexBuffer() noexcept
			: m_vbo {0}
			, m_size {0}
		{
			glCreateBuffers(1, &m_vbo);
		}

		VertexBuffer::VertexBuffer(const unsigned int capacity, const StorageFlag flag) noexcept
			: m_vbo {0}
			, m_size {capacity}
		{
			glCreateBuffers(1, &m_vbo);
			glNamedBufferData(m_vbo, m_size * sizeof(Vertex), nullptr, static_cast<GLenum>(flag));
		}

		VertexBuffer::VertexBuffer(VertexBuffer&& vb) noexcept
		{
			this->destroy();

			this->m_vbo  = vb.m_vbo;
			this->m_size = vb.m_size;

			vb.m_vbo = 0;
		}

		VertexBuffer& VertexBuffer::operator=(VertexBuffer&& vb) noexcept
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

		VertexBuffer::~VertexBuffer() noexcept
		{
			destroy();
		}

		void VertexBuffer::create(std::span<Vertex> vertices, const StorageFlag flag)
		{
			if (!vertices.empty())
			{
				m_size = static_cast<unsigned int>(vertices.size());

				m_vertices.clear();
				m_vertices.reserve(m_size);
				m_vertices.assign(std::make_move_iterator(vertices.begin()), std::make_move_iterator(vertices.end()));

				glNamedBufferData(m_vbo, vertices.size_bytes(), vertices.data(), static_cast<GLenum>(flag));
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Attempted to upload empty vertex buffer.");
			}
		}

		void VertexBuffer::reserve(const unsigned int size, const StorageFlag flag)
		{
			if (size > 0u)
			{
				m_size = size;

				m_vertices.clear();
				m_vertices.reserve(m_size);

				glNamedBufferData(m_vbo, size, nullptr, static_cast<GLenum>(flag));
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Reserved vertex buffer size must be greater than 0.");
			}
		}

		void VertexBuffer::sub_buffer(const unsigned int index, std::span<Vertex> vertices)
		{
			if (!vertices.empty() && (vertices.size() <= (m_vertices.size() - index)))
			{
				glNamedBufferSubData(m_vbo, index * sizeof(Vertex), vertices.size() * sizeof(Vertex), vertices.data());
				std::swap_ranges(m_vertices.begin() + index, m_vertices.begin() + index + vertices.size(), vertices.begin());
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Attempted to buffer outside vertex buffer range.");
			}
		}

		void VertexBuffer::clear() noexcept
		{
			glInvalidateBufferData(m_vbo);
		}

		void VertexBuffer::destroy() noexcept
		{
			if (m_vbo != 0)
			{
				glDeleteBuffers(1, &m_vbo);
				m_vbo = 0;
			}
		}

		unsigned int VertexBuffer::id() const noexcept
		{
			return m_vbo;
		}
	} // namespace graphics
} // namespace galaxy