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
	    : m_z_level {0}
	{
	}

	const unsigned int VertexData::z_level() const noexcept
	{
		return m_z_level;
	}

	qs::VertexBuffer& VertexData::get_vbo() noexcept
	{
		return m_vb;
	}

	qs::VertexArray& VertexData::get_vao() noexcept
	{
		return m_va;
	}

	const unsigned int VertexData::index_count() const noexcept
	{
		return m_ib.count();
	}
} // namespace qs