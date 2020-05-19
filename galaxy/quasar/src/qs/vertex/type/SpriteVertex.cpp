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
		:m_pos(0.0f, 0.0f), m_texels(0.0f, 0.0f), m_opacity(1.0f)
	{
	}

	SpriteVertex::SpriteVertex(const float x, const float y, const float u, const float v, const float opacity) noexcept
		:m_pos(x, y), m_texels(u, v), m_opacity(opacity)
	{
	}
}