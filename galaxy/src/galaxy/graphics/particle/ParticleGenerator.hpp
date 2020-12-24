///
/// ParticleGenerator.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_PARTICLEGENERATOR_HPP_
#define QUASAR_PARTICLEGENERATOR_HPP_

#include <galaxy/graphics/Rect.hpp>
#include <robin_hood.h>

#include "qs/graphics/Particle.hpp"
#include "qs/graphics/ParticleInstance.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// Generates particles using OpenGL instancing and sprite batching.
	///
	class ParticleGenerator final
	{
	public:
		///
		/// Constructor.
		///
		ParticleGenerator();

		///
		/// Copy constructor.
		///
		ParticleGenerator(const ParticleGenerator&) = delete;

		///
		/// Move constructor.
		///
		ParticleGenerator(ParticleGenerator&&);

		///
		/// Copy assignment operator.
		///
		ParticleGenerator& operator=(const ParticleGenerator&) = delete;

		///
		/// Move assignment operator.
		///
		ParticleGenerator& operator=(ParticleGenerator&&);

		///
		/// Destructor.
		///
		~ParticleGenerator();

		///
		/// Create particle generator.
		///
		/// \param particle_sheet Texture spritesheet containing particles for generator to use.
		/// \param emitter_x Set the x position of where to draw particles from.
		/// \param emitter_y Set the y position of where to draw particles from.
		///
		void create(std::string_view particle_sheet, float emitter_x, float emitter_y);

		///
		/// Define a particle type on the texture spritesheet.
		///
		/// \param particle_type Identifier for this texture region.
		/// \param region Texture coords on the spritesheet.
		///
		void define(std::string_view particle_type, pr::Rect<int> region);

		///
		/// \brief Configure the generator with what type of particle to emit, amount, and the min/max offset of the instancing random pos generator.
		///
		/// Sets current particle to particle_type.
		///
		/// \param particle_type Identifier for this texture region.
		///
		void configure(std::string_view particle_type);

		///
		/// \brief Generate particles and offsets linearly.
		///
		/// Sets current particle to particle_type.
		///
		/// \param particle_type Identifier for this texture region.
		/// \param amount Amount of particles to render.
		/// \param max_offset_x Maximum random offset from center on x axis.
		/// \param max_offset_y Maximum random offset from center on y axis.
		/// \param vel_x X axis velocity of particle.
		/// \param vel_y Y axis velocity of particle.
		///
		void gen_linear(std::string_view particle_type, const unsigned int amount, const float max_offset_x, const float max_offset_y, float vel_x, float vel_y);

		///
		/// \brief Generate particles and offsets in a circular pattern.
		///
		/// Sets current particle to particle_type.
		///
		/// \param particle_type Identifier for this texture region.
		/// \param amount Amount of particles to render.
		/// \param radius Radius around emitter to draw particles.
		/// \param vel_x X axis velocity of particle.
		/// \param vel_y Y axis velocity of particle.
		///
		void gen_circular(std::string_view particle_type, const unsigned int amount, const float radius, float vel_x, float vel_y);

		///
		/// Update to process the "life" (opacity) of the particles.
		///
		/// \param dt DeltaTime from gameloop.
		/// \param life Speed at which the particle decays.
		///
		void update(const double dt, const float life);

		///
		/// Update emitter position.
		///
		/// \param emitter_x Set the x position of where to draw particles from.
		/// \param emitter_y Set the y position of where to draw particles from.
		///
		void update_emitter(const float emitter_x, const float emitter_y);

		///
		/// Bind particle generator to active opengl instance.
		///
		void bind();

		///
		/// Unbind particle generator from active opengl instance.
		///
		void unbind();

		///
		/// Get the currently rendering particle.
		///
		/// \return Pointer to active particle type.
		///
		[[nodiscard]] qs::ParticleInstance* get_instance();

		///
		/// Is the current particle lifecycle finished?
		///
		/// \return Const boolean.
		///
		[[nodiscard]] const bool finished() const;

		///
		/// Get the current amount of particles being drawn.
		///
		/// \return Const unsigned integer.
		///
		[[nodiscard]] const unsigned int amount() const;

		///
		/// Get the current OpenGL index element buffer count.
		///
		/// \return Const unsigned integer.
		///
		[[nodiscard]] const unsigned int gl_index_count();

	private:
		///
		/// Set x position of emitter.
		///
		float m_emitter_x;

		///
		/// Set y position of emitter.
		///
		float m_emitter_y;

		///
		/// Amount of particles being generated per instance.
		///
		unsigned int m_amount;

		///
		/// Is the current particle lifecycle finished.
		///
		bool m_finished;

		///
		/// Currently active particle.
		///
		std::string m_current_instance;

		///
		/// Particle generator sprite sheet.
		///
		qs::Texture m_texture;

		///
		/// Last particle accessed.
		///
		std::size_t m_current_particle;

		///
		/// Offsets of currently configured particles.
		///
		std::vector<glm::vec3> m_offsets;

		///
		/// Array of particles being rendererd.
		///
		std::vector<Particle> m_particles;

		///
		/// Array of different particles this generator can produce.
		///
		robin_hood::unordered_map<std::string, ParticleInstance> m_particles_instances;

		///
		/// Region of each particle type.
		///
		robin_hood::unordered_map<std::string, pr::Rect<int>> m_texture_regions;
	};
} // namespace qs

#endif