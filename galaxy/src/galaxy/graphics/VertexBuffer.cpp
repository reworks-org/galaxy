///
/// VertexBuffer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "VertexBuffer.hpp"

namespace galaxy
{
	namespace graphics
	{
		VertexBuffer::VertexBuffer() noexcept
		    : m_vbo {0}, m_size {0}
		{
			glGenBuffers(1, &m_vbo);
		}

		VertexBuffer::VertexBuffer(VertexBuffer&& vb) noexcept
		{
			this->m_vbo      = vb.m_vbo;
			this->m_size     = vb.m_size;
			this->m_vertices = std::move(vb.m_vertices);

			vb.m_vbo = 0;
		}

		VertexBuffer& VertexBuffer::operator=(VertexBuffer&& vb) noexcept
		{
			if (this != &vb)
			{
				this->m_vbo      = vb.m_vbo;
				this->m_size     = vb.m_size;
				this->m_vertices = std::move(vb.m_vertices);

				vb.m_vbo = 0;
			}

			return *this;
		}

		VertexBuffer::~VertexBuffer() noexcept
		{
			destroy();
		}

		void VertexBuffer::create(std::span<Vertex> vertices, bool single_write, unsigned int capacity)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

			const auto draw_type = (single_write == true) ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;

			if (!vertices.empty())
			{
				m_size = static_cast<unsigned int>(vertices.size());

				m_vertices.clear();
				m_vertices.reserve(m_size);
				m_vertices.assign(std::make_move_iterator(vertices.begin()), std::make_move_iterator(vertices.end()));

				glBufferData(GL_ARRAY_BUFFER, vertices.size_bytes(), vertices.data(), draw_type);
			}
			else
			{
				m_size = capacity;
				glBufferData(GL_ARRAY_BUFFER, m_size * sizeof(Vertex), nullptr, draw_type);
			}

			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		void VertexBuffer::sub_buffer(const unsigned int index, std::span<Vertex> vertices)
		{
			if (vertices.size() <= (m_vertices.size() - index))
			{
				glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
				glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(Vertex), vertices.size() * sizeof(Vertex), vertices.data());
				glBindBuffer(GL_ARRAY_BUFFER, 0);

				std::swap_ranges(m_vertices.begin() + index, m_vertices.begin() + index + vertices.size(), vertices.begin());
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Attempted to buffer outside valid vertex buffer data store range.");
			}
		}

		void VertexBuffer::bind() noexcept
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		}

		void VertexBuffer::unbind() noexcept
		{
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		void VertexBuffer::destroy() noexcept
		{
			glDeleteBuffers(1, &m_vbo);
			m_vbo = 0;
		}

		const std::vector<Vertex>& VertexBuffer::get_vertices() const noexcept
		{
			return m_vertices;
		}

		const unsigned int VertexBuffer::id() const noexcept
		{
			return m_vbo;
		}
	} // namespace graphics
} // namespace galaxy