///
/// Particle.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_PARTICLE_HPP_
#define QUASAR_PARTICLE_HPP_

#include "qs/sprite/Sprite.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// Draws an instanced vertex array with a texture.
	///
	class Particle final : public Sprite
	{
	public:
		///
		/// Constructor.
		///
		Particle();

		///
		/// Constructor.
		///
		Particle(const float x_vel, const float y_vel);

		///
		/// Copy constructor.
		///
		Particle(const Particle&) noexcept = delete;

		///
		/// Move constructor.
		///
		Particle(Particle&&);

		///
		/// Copy assignment operator.
		///
		Particle& operator=(const Particle&) noexcept = delete;

		///
		/// Move assignment operator.
		///
		Particle& operator=(Particle&&);

		///
		/// Destructor.
		///
		virtual ~Particle() noexcept = default;

		///
		/// Set velocity of particle(s).
		///
		/// \param x_vel x-axis velocity.
		/// \param y_vel y-axis velocity.
		///
		void set_velocity(const float x_vel, const float y_vel);

		///
		/// Creates the internal vertex array.
		///
		/// \param instances Set of instance coords to spawn each particle at.
		///
		void set_instance(std::span<glm::vec2> instances);

		///
		/// Get amount of particles to draw.
		///
		/// \return Const reference to a glm::vec2.
		///
		const glm::vec2& velocity() const noexcept;

	private:
		///
		/// Velocity of the particle.
		///
		glm::vec2 m_velocity;

		///
		/// InstanceBuffer object.
		///
		qs::InstanceBuffer m_instance_buffer;
	};
} // namespace qs

#endif