///
/// Material.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_LIGHT_MATERIAL_HPP_
#define GALAXY_GRAPHICS_LIGHT_MATERIAL_HPP_

#include <glm/vec3.hpp>

namespace galaxy
{
	namespace light
	{
		class Material final
		{
		public:
			///
			/// Constructor.
			///
			Material() noexcept;

			///
			/// Destructor.
			///
			~Material() noexcept = default;

		public:
			///
			/// Ambient material colour.
			///
			glm::vec3 m_ambient;

			///
			/// Diffuse material colour.
			///
			glm::vec3 m_diffuse;

			///
			/// Specular material colour.
			///
			glm::vec3 m_specular;

			///
			/// Material shininess.
			///
			float m_shininess;
		};
	} // namespace light
} // namespace galaxy

#endif