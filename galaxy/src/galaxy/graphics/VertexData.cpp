///
/// VertexData.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "VertexData.hpp"

namespace galaxy
{
	namespace graphics
	{
		VertexData::VertexData(VertexData&& vd) noexcept
		{
			this->m_ib     = std::move(vd.m_ib);
			this->m_layout = std::move(vd.m_layout);
			this->m_va     = std::move(vd.m_va);
			this->m_vb     = std::move(vd.m_vb);
		}

		VertexData& VertexData::operator=(VertexData&& vd) noexcept
		{
			if (this != &vd)
			{
				this->m_ib     = std::move(vd.m_ib);
				this->m_layout = std::move(vd.m_layout);
				this->m_va     = std::move(vd.m_va);
				this->m_vb     = std::move(vd.m_vb);
			}

			return *this;
		}

		IndexBuffer& VertexData::get_ibo() noexcept
		{
			return m_ib;
		}

		VertexBuffer& VertexData::get_vbo() noexcept
		{
			return m_vb;
		}

		VertexArray& VertexData::get_vao() noexcept
		{
			return m_va;
		}

		const unsigned int VertexData::index_count() const noexcept
		{
			return m_ib.count();
		}
	} // namespace graphics
} // namespace galaxy