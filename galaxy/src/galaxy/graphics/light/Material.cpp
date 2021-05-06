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
		    : m_diffuse_colours {1.0f, 1.0f, 1.0f, 1.0f}, m_specular_colours {0.0f, 0.0f, 0.0f}, m_use_diffuse_texture {true}, m_use_specular_texture {true}, m_use_normal_texture {true}, m_shininess {-1.0f}
		{
		}
	} // namespace light
} // namespace galaxy