///
/// Directional.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_LIGHT_DIRECTIONAL_HPP_
#define GALAXY_GRAPHICS_LIGHT_DIRECTIONAL_HPP_

#include "galaxy/graphics/light/Light.hpp"

namespace galaxy
{
	namespace light
	{
		///
		/// Directional light in a shader.
		///
		class Directional final : public Light
		{
		public:
			///
			/// Constructor.
			///
			Directional() noexcept;

			///
			/// Virtual destructor.
			///
			virtual ~Directional() noexcept = default;

		public:
			///
			/// Direction of the light.
			///
			glm::vec3 m_dir;
		};
	} // namespace light
} // namespace galaxy

#endif