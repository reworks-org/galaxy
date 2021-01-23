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
		VertexData::VertexData() noexcept
		    : m_opacity {1.0f}
		{
		}

		VertexData::VertexData(VertexData&& vd) noexcept
		{
			this->m_ib      = std::move(vd.m_ib);
			this->m_layout  = std::move(vd.m_layout);
			this->m_va      = std::move(vd.m_va);
			this->m_vb      = std::move(vd.m_vb);
			this->m_opacity = vd.m_opacity;
		}

		VertexData& VertexData::operator=(VertexData&& vd) noexcept
		{
			if (this != &vd)
			{
				this->m_ib      = std::move(vd.m_ib);
				this->m_layout  = std::move(vd.m_layout);
				this->m_va      = std::move(vd.m_va);
				this->m_vb      = std::move(vd.m_vb);
				this->m_opacity = vd.m_opacity;
			}

			return *this;
		}

		void VertexData::set_opacity(const float opacity) noexcept
		{
			if (m_opacity > 1.0f)
			{
				m_opacity = 1.0f;
			}
			else if (m_opacity < 0.0f)
			{
				m_opacity = 0.0f;
			}
			else
			{
				m_opacity = opacity;
			}
		}

		const float VertexData::opacity() const noexcept
		{
			return m_opacity;
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