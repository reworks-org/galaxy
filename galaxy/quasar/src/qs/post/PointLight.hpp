///
/// PointLight.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_POINTLIGHT_HPP_
#define QUASAR_POINTLIGHT_HPP_

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "qs/post/PostEffect.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// PostEffect identifier.
	///
	namespace effect
	{
		///
		/// A point of light in GLSL.
		///
		class PointLight final : public PostEffect
		{
		public:
			///
			/// Constructor.
			///
			PointLight();

			///
			/// Destructor.
			///
			virtual ~PointLight() = default;

			///
			/// Z level of light, changes the way the light reflects off the normal map.
			///
			float m_z_level;

			///
			/// XY location of light source.
			///
			glm::vec2 m_pos;

			///
			/// Falloff range for the light source.
			/// Controls intensity.
			///
			glm::vec3 m_falloff;

			///
			/// Colour of the light.
			///
			glm::vec4 m_light_colour;

			///
			/// Colour of the ambient light.
			///
			glm::vec4 m_ambient_colour;

		private:
			///
			/// \brief Set the appropriate uniforms for the shader.
			///
			/// Called internally in the derived effect, then called by Window.
			///
			void apply_uniforms() override;
		};
	} // namespace effect
} // namespace qs

#endif