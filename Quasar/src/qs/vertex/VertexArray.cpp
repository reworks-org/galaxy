///
/// VertexArrary.cpp
/// Quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include "VertexArray.hpp"

namespace qs
{
	VertexArray::VertexArray() noexcept
		:m_id(0)
	{
	}

	VertexArray::VertexArray(const qs::VertexBuffer& vb, const qs::IndexBuffer& ib, const qs::VertexLayout& layout) noexcept
	{
		create(vb, ib, layout);
	}

	void VertexArray::create(const qs::VertexBuffer& vb, const qs::IndexBuffer& ib, const qs::VertexLayout& layout) noexcept
	{
		glGenVertexArrays(1, &m_id); // create the vertex array

		bind();
		vb.bind(); // add vertex buffer
		ib.bind(); // add index buffer

		// Add each attribute in the layout to the vertex array object.
		// I.e. position attribute, then colour attribute of the verticies.
		const auto& attributes = layout.getAttributes();
		unsigned int counter = 0;
		for (const auto& attribute : attributes)
		{
			// Ensures a proper conversion to the void* pointer.
			const void* pointer = static_cast<const char*>(0) + attribute.m_offset;
			
			glVertexAttribPointer(counter, attribute.m_size, attribute.m_type, attribute.m_normalized, layout.stride(), pointer);
			glEnableVertexAttribArray(counter);
			
			++counter;
		}

		// Unbind since we are finished.
		unbind();
		vb.unbind();
		ib.unbind();
	}

	VertexArray::~VertexArray() noexcept
	{
		glDeleteVertexArrays(1, &m_id);
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