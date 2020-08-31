///
/// InstanceVertex.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include "InstanceVertex.hpp"

///
/// Core namespace.
///
namespace qs
{
	InstanceVertex::InstanceVertex() noexcept
	    : m_pos {0.0f, 0.0f}, m_texels {0.0f, 0.0f}, m_offset {0.0f, 0.0f}
	{
	}

	InstanceVertex::InstanceVertex(const float x, const float y, const float u, const float v, const float off_x, const float off_y) noexcept
	    : m_pos {x, y}, m_texels {u, v}, m_offset {off_x, off_y}
	{
	}

	InstanceVertex::InstanceVertex(const InstanceVertex& iv)
	{
		this->m_pos    = iv.m_pos;
		this->m_texels = iv.m_texels;
		this->m_offset = iv.m_offset;
	}

	InstanceVertex::InstanceVertex(InstanceVertex&& iv)
	{
		this->m_pos    = std::move(iv.m_pos);
		this->m_texels = std::move(iv.m_texels);
		this->m_offset = std::move(iv.m_offset);
	}

	InstanceVertex& InstanceVertex::operator=(const InstanceVertex& iv)
	{
		this->m_pos    = iv.m_pos;
		this->m_texels = iv.m_texels;
		this->m_offset = iv.m_offset;

		return *this;
	}

	InstanceVertex& InstanceVertex::operator=(InstanceVertex&& iv)
	{
		if (this != &iv)
		{
			this->m_pos    = std::move(iv.m_pos);
			this->m_texels = std::move(iv.m_texels);
			this->m_offset = std::move(iv.m_offset);
		}

		return *this;
	}

} // namespace qs