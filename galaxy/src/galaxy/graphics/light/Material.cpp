///
/// Material.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "Material.hpp"

namespace galaxy
{
	namespace light
	{
		Material::Material() noexcept
		    : m_ambient {0.0f, 0.0f, 0.0f}, m_diffuse {0.0f, 0.0f, 0.0f}, m_specular {0.0f, 0.0f, 0.0f}, m_shininess {0.0f}
		{
		}

		Material::Material(const nlohmann::json& json)
		{
			const auto& ambient = json.at("ambient");
			m_ambient.x         = ambient.at("x");
			m_ambient.y         = ambient.at("y");
			m_ambient.z         = ambient.at("z");

			const auto& diffuse = json.at("diffuse");
			m_diffuse.x         = diffuse.at("x");
			m_diffuse.y         = diffuse.at("y");
			m_diffuse.z         = diffuse.at("z");

			const auto& specular = json.at("specular");
			m_specular.x         = specular.at("x");
			m_specular.y         = specular.at("y");
			m_specular.z         = specular.at("z");

			m_shininess = json.at("shininess");
		}
	} // namespace light
} // namespace galaxy