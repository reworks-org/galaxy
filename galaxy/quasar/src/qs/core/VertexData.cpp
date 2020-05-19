///
/// VertexData.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include "VertexData.hpp"

///
/// Core namespace.
///
namespace qs
{
	VertexData::VertexData() noexcept
		:m_zLevel(0)
	{
	}

	const unsigned int VertexData::getZLevel() const noexcept
	{
		return m_zLevel;
	}

	qs::VertexBuffer& VertexData::getVBO() noexcept
	{
		return m_vertexBuffer;
	}

	qs::VertexArray& VertexData::getVAO() noexcept
	{
		return m_vertexArray;
	}
	
	const unsigned int VertexData::getCount() const noexcept
	{
		return m_indexBuffer.getCount();
	}
}