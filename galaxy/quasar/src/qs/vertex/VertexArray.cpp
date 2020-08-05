///
/// VertexArrary.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include "VertexArray.hpp"

///
/// Core namespace.
///
namespace qs
{
	VertexArray::VertexArray() noexcept
	    : m_id {0}
	{
		glGenVertexArrays(1, &m_id);
	}

	VertexArray::~VertexArray() noexcept
	{
		glDeleteVertexArrays(1, &m_id);
	}

	void VertexArray::bind() noexcept
	{
		glBindVertexArray(m_id);
	}

	void VertexArray::unbind() noexcept
	{
		glBindVertexArray(0);
	}
	
	void VertexArray::change_divisor(pr::positive_uint auto index, pr::positive_uint auto div) noexcept
	{
		glBindVertexArray(m_id);
		glVertexAttribDivisor(index, div);
		glBindVertexArray(0):
	}
} // namespace qs