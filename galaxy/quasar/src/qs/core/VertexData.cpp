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
	    : m_z_level {0}, m_opacity {1.0f}
	{
	}

	VertexData::VertexData(VertexData&& vd)
	{
		this->m_ib      = std::move(vd.m_ib);
		this->m_layout  = std::move(vd.m_layout);
		this->m_va      = std::move(vd.m_va);
		this->m_vb      = std::move(vd.m_vb);
		this->m_z_level = vd.m_z_level;
		this->m_opacity = vd.m_opacity;
	}

	VertexData& VertexData::operator=(VertexData&& vd)
	{
		if (this != &vd)
		{
			this->m_ib      = std::move(vd.m_ib);
			this->m_layout  = std::move(vd.m_layout);
			this->m_va      = std::move(vd.m_va);
			this->m_vb      = std::move(vd.m_vb);
			this->m_z_level = vd.m_z_level;
			this->m_opacity = vd.m_opacity;
		}

		return *this;
	}

	void VertexData::set_z_level(const unsigned int z_level) noexcept
	{
		m_z_level = z_level;
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

	const unsigned int VertexData::z_level() const noexcept
	{
		return m_z_level;
	}

	const float VertexData::opacity() const noexcept
	{
		return m_opacity;
	}

	qs::IndexBuffer& VertexData::get_ibo() noexcept
	{
		return m_ib;
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