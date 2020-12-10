///
/// Body.cpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#include "Body.hpp"

///
/// Core namespace.
///
namespace rs
{
	Body::Body()
	    : m_velocity {0.0f, 0.0f}, m_impulse {0.0f, 0.0f}, m_mass {1.0f}, m_restitution {1.0f}
	{
	}
} // namespace rs