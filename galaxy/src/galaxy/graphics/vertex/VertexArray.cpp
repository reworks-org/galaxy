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
		    : m_id {0}, m_counter {0}
		{
			glGenVertexArrays(1, &m_id);
		}

		VertexArray::VertexArray(VertexArray&& va) noexcept
		{
			this->m_id      = va.m_id;
			this->m_counter = va.m_counter;

			va.m_id      = 0;
			va.m_counter = 0;
		}

		VertexArray& VertexArray::operator=(VertexArray&& va) noexcept
		{
			if (this != &va)
			{
				this->m_id      = va.m_id;
				this->m_counter = va.m_counter;

				va.m_id      = 0;
				va.m_counter = 0;
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
	} // namespace graphics
} // namespace galaxy