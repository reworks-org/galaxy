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

			m_layout.add<graphics::VertexAttributes::POSITION>(2);
			m_layout.add<graphics::VertexAttributes::TEXEL>(2);
			m_layout.add<graphics::VertexAttributes::COLOUR>(4);
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

		void VertexArray::create(VertexBuffer& vb, IndexBuffer& ib)
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
			for (const auto& attribute : m_layout.get_attributes())
			{
				glEnableVertexAttribArray(m_counter);
				glVertexAttribPointer(m_counter, attribute.m_size, attribute.m_type, attribute.m_normalized, sizeof(Vertex), (GLvoid*)attribute.m_offset);

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

		void VertexArray::set_instanced(InstanceBuffer& ibo) noexcept
		{
			bind();

			glEnableVertexAttribArray(m_counter);
			ibo.bind();

			m_layout.add<graphics::VertexAttributes::INSTANCE_OFFSET>(2);
			const auto& attribute = m_layout.get_attributes().back();

			glVertexAttribPointer(m_counter, attribute.m_size, attribute.m_type, attribute.m_normalized, sizeof(Vertex), (GLvoid*)attribute.m_offset);
			glVertexBindingDivisor(m_counter, 1);

			unbind();
			ibo.unbind();
		}

		const int VertexArray::index_count() const noexcept
		{
			return m_ibo.index_count();
		}

		const unsigned int VertexArray::id() const noexcept
		{
			return m_vao;
		}

		const unsigned int VertexArray::vbo() const noexcept
		{
			return m_vbo.id();
		}
	} // namespace graphics
} // namespace galaxy