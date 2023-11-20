///
/// Particle.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_PARTICLE_HPP_
#define GALAXY_GRAPHICS_PARTICLE_HPP_

#include <compare>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace galaxy
{
	namespace graphics
	{
		///
		/// Data for a particle in a particle effect.
		///
		class Particle final
		{
		  public:
			///
			/// Constructor.
			///
			Particle();

			///
			/// Comparison operator.
			///
			[[nodiscard]] auto operator<=>(const Particle&) const = default;

		  public:
			///
			/// Position.
			///
			glm::vec2 m_pos;

			///
			/// Velocity.
			///
			glm::vec2 m_vel;

			///
			/// Scale of particle.
			///
			glm::vec2 m_scale;

			///
			/// Colour tinting of particle texture.
			///
			glm::vec4 m_colour;

			///
			/// Duration of particle visibility.
			///
			float m_life;
		};
	} // namespace graphics
} // namespace galaxy

#endif
