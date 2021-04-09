///
/// Material.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_LIGHT_MATERIAL_HPP_
#define GALAXY_GRAPHICS_LIGHT_MATERIAL_HPP_

#include <glm/vec3.hpp>
#include <nlohmann/json_fwd.hpp>

namespace galaxy
{
	namespace graphics
	{
		class Material final
		{
		public:
			Material() noexcept;
			Material(const nlohmann::json& json);
			~Material() noexcept = default;

		public:
			glm::vec3 m_ambient;
			glm::vec3 m_diffuse;
			glm::vec3 m_specular;
			float m_shininess;
		};
	} // namespace graphics
} // namespace galaxy

#endif