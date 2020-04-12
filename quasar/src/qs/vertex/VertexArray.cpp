///
/// VertexArrary.cpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include "qs/core/Transform.hpp"
#include "qs/vertex/VertexLayout.hpp"

#include "VertexArray.hpp"

///
/// Core namespace.
///
namespace qs
{
	VertexArray::VertexArray() noexcept
		:m_id(0)
	{
		glGenVertexArrays(1, &m_id);
	}

	VertexArray::VertexArray(qs::VertexBuffer& vb, qs::IndexBuffer& ib, qs::VertexLayout& layout) noexcept
		:m_id(0)
	{
		glGenVertexArrays(1, &m_id);
		create(vb, ib, layout);
	}

	VertexArray::~VertexArray() noexcept
	{
		glDeleteVertexArrays(1, &m_id);
	}

	void VertexArray::create(qs::VertexBuffer& vb, qs::IndexBuffer& ib, qs::VertexLayout& layout) noexcept
	{
		bind();
		vb.bind();
		ib.bind();

		// Add each attribute in the layout to the vertex array object.
		// I.e. position attribute, then colour attribute of the verticies.
		const auto& attributes = layout.getAttributes();
		unsigned int counter = 0;
		for (const auto& attribute : attributes)
		{
			glVertexAttribPointer(counter, attribute.m_size, attribute.m_type, attribute.m_normalized, sizeof(qs::Vertex), (GLvoid*)attribute.m_offset);
			glEnableVertexAttribArray(counter);

			++counter;
		}

		unbind();
		vb.unbind();
		ib.unbind();
	}

	void VertexArray::bind() noexcept
	{
		glBindVertexArray(m_id);
	}

	void VertexArray::unbind() noexcept
	{
		glBindVertexArray(0);
	}
}