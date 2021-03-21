///
/// Particle.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_PARTICLE_PARTICLE_HPP_
#define GALAXY_GRAPHICS_PARTICLE_PARTICLE_HPP_

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
			/// Argument Constructor.
			///
			Particle(const float x_vel, const float y_vel) noexcept;

			///
			/// Copy constructor.
			///
			Particle(const Particle&) noexcept;

			///
			/// Move constructor.
			///
			Particle(Particle&&) noexcept;

			///
			/// Copy assignment operator.
			///
			Particle& operator=(const Particle&) noexcept;

			///
			/// Move assignment operator.
			///
			Particle& operator=(Particle&&) noexcept;

			///
			/// Destructor.
			///
			~Particle() noexcept = default;

			///
			/// Set position of particle.
			///
			/// \param x x-axis.
			/// \param y y-axis.
			///
			void set_position(const float x, const float y) noexcept;

			///
			/// Set velocity of particle.
			///
			/// \param x_vel x-axis velocity.
			/// \param y_vel y-axis velocity.
			///
			void set_velocity(const float x_vel, const float y_vel) noexcept;

			///
			/// Move particle across screen.
			///
			/// \param dt Gameloop delta time.
			///
			void move(const float dt) noexcept;

			///
			/// Get position of particle.
			///
			/// \return Const reference to a glm::vec2.
			///
			[[nodiscard]] const glm::vec2& pos() const noexcept;

			///
			/// Get velocity of particle.
			///
			/// \return Const reference to a glm::vec2.
			///
			[[nodiscard]] const glm::vec2& velocity() const noexcept;

			///
			/// Get angle of travel of particle.
			///
			/// \return Const float.
			///
			[[nodiscard]] const float angle() const noexcept;

		public:
			///
			/// Lifespan of particle. (i.e. opacity). 0.0f - 1.0f.
			///
			float m_life;

		private:
			///
			/// Angle of particle.
			///
			float m_angle;

			///
			/// Position of particle.
			///
			glm::vec2 m_position;

			///
			/// Velocity of the particle.
			///
			glm::vec2 m_velocity;
		};
	} // namespace graphics
} // namespace galaxy

#endif