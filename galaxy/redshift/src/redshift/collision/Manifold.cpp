///
/// Manifold.cpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#include "Manifold.hpp"

///
/// Core namespace.
///
namespace rs
{
	Manifold::Manifold()
	    : m_penetration {0.0f}, m_normal {1.0f, 1.0f}
	{
	}
} // namespace rs