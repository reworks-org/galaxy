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
	SpriteVertex::SpriteVertex() noexcept
	    : m_pos {0.0f, 0.0f}, m_texels {0.0f, 0.0f}, m_opacity {1.0f}
	{
	}

	SpriteVertex::SpriteVertex(const float x, const float y, const float u, const float v, const float opacity) noexcept
	    : m_pos {x, y}, m_texels {u, v}
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

	SpriteVertex::SpriteVertex(const SpriteVertex& sv)
	{
		this->m_pos     = sv.m_pos;
		this->m_texels  = sv.m_texels;
		this->m_opacity = sv.m_opacity;
	}

	SpriteVertex::SpriteVertex(SpriteVertex&& sv)
	{
		this->m_pos     = std::move(sv.m_pos);
		this->m_texels  = std::move(sv.m_texels);
		this->m_opacity = sv.m_opacity;
	}

	SpriteVertex& SpriteVertex::operator=(const SpriteVertex& sv)
	{
		this->m_pos     = sv.m_pos;
		this->m_texels  = sv.m_texels;
		this->m_opacity = sv.m_opacity;

		return *this;
	}

	SpriteVertex& SpriteVertex::operator=(SpriteVertex&& sv)
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