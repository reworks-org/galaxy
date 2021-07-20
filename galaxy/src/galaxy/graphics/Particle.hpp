///
/// Particle.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_PARTICLE_HPP_
#define GALAXY_GRAPHICS_PARTICLE_HPP_

#include <glm/vec2.hpp>

namespace galaxy
{
	namespace graphics
	{
		///
		/// Manages particle effect instance data.
		///
		class Particle final
		{
		public:
			///
			/// Constructor.
			///
			Particle() noexcept;

			///
			/// Move constructor.
			///
			Particle(Particle&&) noexcept;

			///
			/// Copy constructor.
			///
			Particle(const Particle&) noexcept;

			///
			/// Move assignment operator.
			///
			Particle& operator=(Particle&&) noexcept;

			///
			/// Copy assignment operator.
			///
			Particle& operator=(const Particle&) noexcept;

			///
			/// Destructor.
			///
			~Particle() noexcept = default;

			///
			/// Move particle across screen.
			///
			/// \param dt Gameloop delta time.
			///
			void move(const float dt) noexcept;

		public:
			///
			/// Lifespan of particle.
			///
			float m_life;

			///
			/// Angle of particle. In radians.
			///
			float m_angle;

			///
			/// Instance offset of each particle.
			///
			glm::vec2 m_offset;

			///
			/// Velocity of the particle.
			///
			glm::vec2 m_velocity;
		};
	} // namespace graphics
} // namespace galaxy

#endif