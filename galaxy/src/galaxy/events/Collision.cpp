///
/// Collision.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Collision.hpp"

namespace galaxy
{
	namespace events
	{
		Collision::Collision(b2Body* a, b2Body* b) noexcept
		    : m_a {a}, m_b {b}
		{
		}
	} // namespace events
} // namespace galaxy