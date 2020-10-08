///
/// Particle.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_PARTICLE_HPP_
#define QUASAR_PARTICLE_HPP_

#include <glm/vec2.hpp>

///
/// Core namespace.
///
namespace qs
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
		Particle();

		///
		/// Argument Constructor.
		///
		Particle(const float x_vel, const float y_vel);

		///
		/// Copy constructor.
		///
		Particle(const Particle&);

		///
		/// Move constructor.
		///
		Particle(Particle&&);

		///
		/// Copy assignment operator.
		///
		Particle& operator=(const Particle&);

		///
		/// Move assignment operator.
		///
		Particle& operator=(Particle&&);

		///
		/// Destructor.
		///
		~Particle() = default;

		///
		/// Set position of particle.
		///
		/// \param x x-axis.
		/// \param y y-axis.
		///
		void set_position(const float x, const float y);

		///
		/// Set velocity of particle.
		///
		/// \param x_vel x-axis velocity.
		/// \param y_vel y-axis velocity.
		///
		void set_velocity(const float x_vel, const float y_vel);

		///
		/// Move particle across screen.
		///
		/// \param dt Gameloop delta time.
		///
		void move(const float dt);

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
		/// Lifespan of particle. (i.e. opacity). 0.0f - 1.0f.
		///
		float m_life;

	private:
		///
		/// Position of particle.
		///
		glm::vec2 m_position;

		///
		/// Velocity of the particle.
		///
		glm::vec2 m_velocity;
	};
} // namespace qs

#endif