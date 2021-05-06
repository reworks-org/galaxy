///
/// Material.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_LIGHT_MATERIAL_HPP_
#define GALAXY_GRAPHICS_LIGHT_MATERIAL_HPP_

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "galaxy/graphics/texture/Texture.hpp"

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
			/// Diffuse material texture.
			///
			graphics::Texture m_diffuse;

			///
			/// Specular material texture.
			///
			graphics::Texture m_specular;

			///
			/// Normalmap material texture.
			///
			graphics::Texture m_normal;

			///
			/// Diffuse (ambient) colours.
			///
			glm::vec4 m_diffuse_colours;

			///
			/// Specular colours.
			///
			glm::vec3 m_specular_colours;

			///
			/// True to use texture, false to use colours.
			///
			bool m_use_diffuse_texture;

			///
			/// True to use texture, false to use colours.
			///
			bool m_use_specular_texture;

			///
			/// True to use texture, false to use colours.
			///
			bool m_use_normal_texture;

			///
			/// Material shininess.
			///
			float m_shininess;
		};
	} // namespace light
} // namespace galaxy

#endif