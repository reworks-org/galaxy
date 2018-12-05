///
/// VertexAttribute.cpp
/// Quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include "VertexAttribute.hpp"

namespace qs
{
	VertexAttribute::VertexAttribute() noexcept
		:m_size(0), m_type(0), m_normalized(0), m_offset(0)
	{
	}

	VertexAttribute::VertexAttribute(int size, unsigned int type, unsigned char normalized, unsigned int offset) noexcept
		:m_size(size), m_type(type), m_normalized(normalized), m_offset(offset)
	{
	}
}