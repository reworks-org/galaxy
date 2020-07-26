///
/// LightSource.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include "LightSource.hpp"

///
/// Core namespace.
///
namespace qs
{
	LightSource::LightSource() noexcept
	    : m_z_level {0.0f}, m_pos {0.0f, 0.0f}, m_falloff {0.0f, 0.0f, 1.0f}, m_light_colour {1.0f}, m_ambient_colour {1.0f}
	{
	}
} // namespace qs