///
/// Material.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Material.hpp"

namespace galaxy
{
	namespace light
	{
		Material::Material() noexcept
		    : m_ambient {0.0f, 0.0f, 0.0f}, m_diffuse {0.0f, 0.0f, 0.0f}, m_specular {0.0f, 0.0f, 0.0f}, m_shininess {0.0f}
		{
		}
	} // namespace light
} // namespace galaxy