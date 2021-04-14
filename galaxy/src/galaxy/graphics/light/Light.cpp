///
/// Light.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Light.hpp"

namespace galaxy
{
	namespace light
	{
		Light::Light() noexcept
		    : m_ambient_intensity {0.2f}, m_diffuse_intensity {0.5f}, m_specular_intensity {1.0f}
		{
		}
	} // namespace light
} // namespace galaxy