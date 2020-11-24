///
/// BatchedVertex.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include "BatchedVertex.hpp"

///
/// Core namespace.
///
namespace qs
{
	BatchedVertex::BatchedVertex()
	    : m_pos {0.0f, 0.0f}, m_texels {0.0f, 0.0f}, m_opacity {1.0f}
	{
	}

	BatchedVertex::BatchedVertex(const float x, const float y, const float u, const float v, const float opacity)
	    : m_pos {x, y}, m_texels {u, v}, m_opacity {opacity}

	{
	}

	BatchedVertex::BatchedVertex(const BatchedVertex& sv)
	{
		this->m_pos     = sv.m_pos;
		this->m_texels  = sv.m_texels;
		this->m_opacity = sv.m_opacity;
	}

	BatchedVertex::BatchedVertex(BatchedVertex&& sv)
	{
		this->m_pos     = std::move(sv.m_pos);
		this->m_texels  = std::move(sv.m_texels);
		this->m_opacity = sv.m_opacity;
	}

	BatchedVertex& BatchedVertex::operator=(const BatchedVertex& sv)
	{
		this->m_pos     = sv.m_pos;
		this->m_texels  = sv.m_texels;
		this->m_opacity = sv.m_opacity;

		return *this;
	}

	BatchedVertex& BatchedVertex::operator=(BatchedVertex&& sv)
	{
		if (this != &sv)
		{
			this->m_pos     = std::move(sv.m_pos);
			this->m_texels  = std::move(sv.m_texels);
			this->m_opacity = sv.m_opacity;
		}

		return *this;
	}
} // namespace qs