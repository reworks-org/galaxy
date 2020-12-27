///
/// BatchedVertex.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "BatchedVertex.hpp"

namespace galaxy
{
	namespace graphics
	{
		BatchedVertex::BatchedVertex()
		    : m_pos {0.0f, 0.0f}, m_texels {0.0f, 0.0f}, m_opacity {1.0f}
		{
		}

		BatchedVertex::BatchedVertex(const float x, const float y, const float u, const float v, const float opacity)
		    : m_pos {x, y}, m_texels {u, v}, m_opacity {opacity}

		{
		}
	} // namespace graphics
} // namespace galaxy