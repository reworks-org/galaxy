///
/// SpriteVertex.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include "SpriteVertex.hpp"

///
/// Core namespace.
///
namespace qs
{
	SpriteVertex::SpriteVertex()
	    : m_pos {0.0f, 0.0f}, m_texels {0.0f, 0.0f}
	{
	}

	SpriteVertex::SpriteVertex(const float x, const float y, const float u, const float v)
	    : m_pos {x, y}, m_texels {u, v}

	{
	}

	SpriteVertex::SpriteVertex(const SpriteVertex& sv)
	{
		this->m_pos    = sv.m_pos;
		this->m_texels = sv.m_texels;
	}

	SpriteVertex::SpriteVertex(SpriteVertex&& sv)
	{
		this->m_pos    = std::move(sv.m_pos);
		this->m_texels = std::move(sv.m_texels);
	}

	SpriteVertex& SpriteVertex::operator=(const SpriteVertex& sv)
	{
		this->m_pos    = sv.m_pos;
		this->m_texels = sv.m_texels;

		return *this;
	}

	SpriteVertex& SpriteVertex::operator=(SpriteVertex&& sv)
	{
		if (this != &sv)
		{
			this->m_pos    = std::move(sv.m_pos);
			this->m_texels = std::move(sv.m_texels);
		}

		return *this;
	}
} // namespace qs