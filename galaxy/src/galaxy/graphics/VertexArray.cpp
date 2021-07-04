///
/// VertexArrary.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "VertexArray.hpp"

namespace galaxy
{
	namespace graphics
	{
		VertexArray::VertexArray() noexcept
		    : m_vao {0}, m_counter {0}
		{
			glGenVertexArrays(1, &m_vao);
		}

		VertexArray::VertexArray(VertexArray&& va) noexcept
		{
			this->m_vao     = va.m_vao;
			this->m_counter = va.m_counter;
			this->m_vbo     = std::move(va.m_vbo);
			this->m_ibo     = std::move(va.m_ibo);

			va.m_vao     = 0;
			va.m_counter = 0;
		}

		VertexArray& VertexArray::operator=(VertexArray&& va) noexcept
		{
			if (this != &va)
			{
				this->m_vao     = va.m_vao;
				this->m_counter = va.m_counter;
				this->m_vbo     = std::move(va.m_vbo);
				this->m_ibo     = std::move(va.m_ibo);

				va.m_vao     = 0;
				va.m_counter = 0;
			}

			return *this;
		}

		VertexArray::~VertexArray() noexcept
		{
			glDeleteVertexArrays(1, &m_vao);
		}

		void VertexArray::create(VertexBuffer& vb, IndexBuffer& ib, const VertexLayout& layout)
		{
			// Prepare for new data.
			m_vbo.destroy();
			m_ibo.destroy();

			m_vbo = std::move(vb);
			m_ibo = std::move(ib);

			bind();
			m_vbo.bind();
			m_ibo.bind();

			// Add each attribute in the layout to the vertex array object.
			// I.e. position attribute, then colour attribute of the verticies.
			const auto& attributes = layout.get_attributes();
			for (const auto& attribute : attributes)
			{
				glVertexAttribPointer(m_counter, attribute.m_size, attribute.m_type, attribute.m_normalized, sizeof(Vertex), (GLvoid*)attribute.m_offset);
				glEnableVertexAttribArray(m_counter);

				++m_counter;
			}

			unbind();
			vb.unbind();
			ib.unbind();
		}

		void VertexArray::bind() noexcept
		{
			glBindVertexArray(m_vao);
		}

		void VertexArray::unbind() noexcept
		{
			glBindVertexArray(0);
		}

		const unsigned int VertexArray::count() const noexcept
		{
			return m_ibo.count();
		}

		const unsigned int VertexArray::vbo() const noexcept
		{
			return m_vbo.id();
		}
	} // namespace graphics
} // namespace galaxy