///
/// Manifold.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Manifold.hpp"

namespace galaxy
{
	namespace physics
	{
		Manifold::Manifold() noexcept
		    : m_penetration {0.0f}, m_normal {1.0f, 1.0f}
		{
		}
	} // namespace physics
} // namespace galaxy