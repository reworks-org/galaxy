///
/// VertexArrary.cpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include "VertexArray.hpp"

///
/// Core namespace.
///
namespace qs
{
	VertexArray::VertexArray() noexcept
		:m_id(0)
	{
	}

	VertexArray::VertexArray(const qs::VertexBuffer& vb, const qs::VertexLayout& layout) noexcept
	{
		create(vb, layout);
	}

	VertexArray::VertexArray(const qs::VertexBuffer& vb, const qs::IndexBuffer& ib, const qs::VertexLayout& layout) noexcept
		:m_id(0)
	{
		create(vb, layout);
		addIndexBuffer(ib);
	}

	VertexArray::~VertexArray() noexcept
	{
		glDeleteVertexArrays(1, &m_id);
	}

	void VertexArray::create(const qs::VertexBuffer& vb, const qs::VertexLayout& layout) noexcept
	{
		glGenVertexArrays(1, &m_id); // create the vertex array

		bind();
		vb.bind(); // add vertex buffer

		// Add each attribute in the layout to the vertex array object.
		// I.e. position attribute, then colour attribute of the verticies.
		const auto& attributes = layout.getAttributes();
		unsigned int counter = 0;
		for (const auto& attribute : attributes)
		{
			glVertexAttribPointer(counter, attribute.m_size, attribute.m_type, attribute.m_normalized, layout.stride() * sizeof(GLfloat), (GLvoid*)attribute.m_offset);
			glEnableVertexAttribArray(counter);

			++counter;
		}
	}

	void VertexArray::addIndexBuffer(const qs::IndexBuffer& ib)
	{
		bind();
		ib.bind(); // add index buffer
	}

	void VertexArray::bind() const noexcept
	{
		glBindVertexArray(m_id);
	}

	void VertexArray::unbind() const noexcept
	{
		glBindVertexArray(0);
	}
}