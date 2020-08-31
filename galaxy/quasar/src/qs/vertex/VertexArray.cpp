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

	VertexArray::VertexArray(VertexArray&& va)
	{
		this->m_id = va.m_id;
		va.m_id    = 0;
	}

	VertexArray& VertexArray::operator=(VertexArray&& va)
	{
		if (this != &va)
		{
			this->m_id = va.m_id;
			va.m_id    = 0;
		}

		return *this;
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

	void VertexArray::change_divisor(unsigned int index, unsigned int div) noexcept
	{
		glBindVertexArray(m_id);
		glVertexAttribDivisor(index, div);
		glBindVertexArray(0);
	}
} // namespace qs