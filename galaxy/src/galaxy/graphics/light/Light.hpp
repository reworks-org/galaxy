///
/// Light.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_LIGHT_LIGHT_HPP_
#define GALAXY_GRAPHICS_LIGHT_LIGHT_HPP_

#include <glm/vec3.hpp>

namespace galaxy
{
	namespace light
	{
		///
		/// Base class for light its different types of emitters i.e. spot, point, etc.
		///
		class Light
		{
		public:
			///
			/// Virtual destructor.
			///
			virtual ~Light() noexcept = default;

		protected:
			///
			/// Constructor.
			///
			Light() noexcept;

		public:
			///
			/// Intensity of ambient light.
			///
			glm::vec3 m_ambient_intensity;

			///
			/// Intensity of diffuse light.
			///
			glm::vec3 m_diffuse_intensity;

			///
			/// Intensity of specular light.
			///
			glm::vec3 m_specular_intensity;
		};
	} // namespace light
} // namespace galaxy

#endif