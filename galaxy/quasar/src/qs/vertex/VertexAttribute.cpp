///
/// VertexAttribute.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include "VertexAttribute.hpp"

///
/// Core namespace.
///
namespace qs
{
	VertexAttribute::VertexAttribute()
	    : m_size {0}, m_type {0}, m_normalized {0}, m_offset {0}
	{
	}

	VertexAttribute::VertexAttribute(const unsigned int size, const unsigned int type, unsigned char normalized, const unsigned int offset)
	    : m_size {size}, m_type {type}, m_normalized {normalized}, m_offset {offset}
	{
	}
} // namespace qs