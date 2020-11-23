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
	VertexData::VertexData()
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

	void VertexData::set_z_level(const unsigned int z_level)
	{
		m_z_level = z_level;
	}

	void VertexData::set_opacity(const float opacity)
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

	const unsigned int VertexData::z_level() const
	{
		return m_z_level;
	}

	const float VertexData::opacity() const
	{
		return m_opacity;
	}

	qs::IndexBuffer& VertexData::get_ibo()
	{
		return m_ib;
	}

	qs::VertexBuffer& VertexData::get_vbo()
	{
		return m_vb;
	}

	qs::VertexArray& VertexData::get_vao()
	{
		return m_va;
	}

	const unsigned int VertexData::index_count() const
	{
		return m_ib.count();
	}
} // namespace qs