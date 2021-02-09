///
/// FinishCollision.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "FinishCollision.hpp"

namespace galaxy
{
	namespace events
	{
		FinishCollision::FinishCollision(b2Body* a, b2Body* b) noexcept
		    : m_a {a}, m_b {b}
		{
		}
	} // namespace events
} // namespace galaxy