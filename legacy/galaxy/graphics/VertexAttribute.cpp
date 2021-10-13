///
/// VertexAttribute.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "VertexAttribute.hpp"

namespace galaxy
{
	namespace graphics
	{
		VertexAttribute::VertexAttribute() noexcept
			: m_size {0}
			, m_type {0}
			, m_normalized {0}
			, m_offset {0}
		{
		}

		VertexAttribute::VertexAttribute(const int size, const unsigned int type, unsigned char normalized, const std::size_t offset) noexcept
			: m_size {size}
			, m_type {type}
			, m_normalized {normalized}
			, m_offset {offset}
		{
		}

		VertexAttribute::VertexAttribute(VertexAttribute&& va) noexcept
		{
			this->m_normalized = va.m_normalized;
			this->m_offset     = va.m_offset;
			this->m_size       = va.m_size;
			this->m_type       = va.m_type;
		}

		VertexAttribute& VertexAttribute::operator=(VertexAttribute&& va) noexcept
		{
			if (this != &va)
			{
				this->m_normalized = va.m_normalized;
				this->m_offset     = va.m_offset;
				this->m_size       = va.m_size;
				this->m_type       = va.m_type;
			}

			return *this;
		}
	} // namespace graphics
} // namespace galaxy