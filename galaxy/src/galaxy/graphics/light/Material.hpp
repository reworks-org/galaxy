///
/// Material.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_LIGHT_MATERIAL_HPP_
#define GALAXY_GRAPHICS_LIGHT_MATERIAL_HPP_

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
			/// Material shininess.
			///
			float m_shininess;
		};
	} // namespace light
} // namespace galaxy

#endif