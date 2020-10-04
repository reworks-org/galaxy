///
/// ParticleGenerator.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_PARTICLEGENERATOR_HPP_
#define QUASAR_PARTICLEGENERATOR_HPP_

#include <protostar/graphics/Rect.hpp>
#include <protostar/math/Random.hpp>

#include "qs/graphics/Particle.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// Type tag for specifying a horizontal type particle generator.
	///
	struct HorizontalGen
	{
		HorizontalGen() = delete;
	};

	///
	/// Type tag for specifying a vertical type particle generator.
	///
	struct VerticalGen
	{
		VerticalGen() = delete;
	};

	///
	/// Type tag for specifying a circular type particle generator.
	///
	struct CircularGen
	{
		CircularGen() = delete;
	};

	///
	/// Concept to ensure particle gen is locked to generator type.
	///
	template<typename Type>
	concept particle_type = (std::is_same<Type, HorizontalGen>::value ||
				 std::is_same<Type, VerticalGen>::value ||
				 std::is_same<Type, CircularGen>::value);

	///
	/// Generates particles using OpenGL instancing and sprite batching.
	///
	template<particle_type ParticleGenType>
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
		/// \brief Generate particles and offsets.
		///
		/// Sets current particle to particle_type.
		///
		/// \param particle_type Identifier for this texture region.
		/// \param amount Amount of particles to render.
		/// \param min_offset Minimium distance from center.
		/// \param max_offset Maximum distance from center.
		/// \param vel_x X axis velocity of particle.
		/// \param vel_y Y axis velocity of particle.
		///
		void generate(std::string_view particle_type, const unsigned int amount, const float min_offset, const float max_offset, const float vel_x, const float vel_y);

		///
		/// Update to process the "life" (opacity) of the particles.
		///
		/// \param dt DeltaTime from gameloop.
		/// \param life Speed at which the particle decays.
		///
		void update(const double dt, const double life);

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
		/// Get the current amount of particles being drawn.
		///
		/// \return Const unsigned integer.
		///
		[[nodiscard]] const unsigned int amount() const noexcept;

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
		std::vector<glm::vec2> m_offsets;

		///
		/// Array of particles being rendererd.
		///
		std::vector<Particle> m_particles;

		///
		/// Array of different particles this generator can produce.
		///
		std::unordered_map<std::string, ParticleInstance> m_particles_instances;

		///
		/// Region of each particle type.
		///
		std::unordered_map<std::string, pr::Rect<int>> m_texture_regions;
	};

	template<particle_type ParticleGenType>
	inline ParticleGenerator<ParticleGenType>::ParticleGenerator()
	    : m_emitter_x {0.0f}, m_emitter_y {0.0f}, m_amount {0}, m_current_instance {""}, m_current_particle {0}
	{
	}

	template<particle_type ParticleGenType>
	inline ParticleGenerator<ParticleGenType>::ParticleGenerator(ParticleGenerator<ParticleGenType>&& pg)
	{
		this->m_emitter_x           = pg.m_emitter_x;
		this->m_emitter_y           = pg.m_emitter_y;
		this->m_amount              = pg.m_amount;
		this->m_current_instance    = pg.m_current_instance;
		this->m_texture             = std::move(pg.m_texture);
		this->m_current_particle    = pg.m_current_particle;
		this->m_offsets             = std::move(pg.m_offsets);
		this->m_particles           = std::move(pg.m_particles);
		this->m_particles_instances = std::move(pg.m_particles_instances);
		this->m_texture_regions     = std::move(pg.m_texture_regions);
	}

	template<particle_type ParticleGenType>
	inline ParticleGenerator<ParticleGenType>& ParticleGenerator<ParticleGenType>::operator=(ParticleGenerator<ParticleGenType>&& pg)
	{
		if (this != &pg)
		{
			this->m_emitter_x           = pg.m_emitter_x;
			this->m_emitter_y           = pg.m_emitter_y;
			this->m_amount              = pg.m_amount;
			this->m_current_instance    = pg.m_current_instance;
			this->m_texture             = std::move(pg.m_texture);
			this->m_current_particle    = pg.m_current_particle;
			this->m_offsets             = std::move(pg.m_offsets);
			this->m_particles           = std::move(pg.m_particles);
			this->m_particles_instances = std::move(pg.m_particles_instances);
			this->m_texture_regions     = std::move(pg.m_texture_regions);
		}

		return *this;
	}

	template<particle_type ParticleGenType>
	inline ParticleGenerator<ParticleGenType>::~ParticleGenerator()
	{
		m_offsets.clear();
		m_particles.clear();
		m_particles_instances.clear();
		m_texture_regions.clear();
	}

	template<particle_type ParticleGenType>
	inline void ParticleGenerator<ParticleGenType>::create(std::string_view particle_sheet, float emitter_x, float emitter_y)
	{
		m_emitter_x = emitter_x;
		m_emitter_y = emitter_y;
		m_texture.load(particle_sheet);
	}

	template<particle_type ParticleGenType>
	inline void ParticleGenerator<ParticleGenType>::define(std::string_view particle_type, pr::Rect<int> region)
	{
		m_texture_regions.emplace(static_cast<std::string>(particle_type), std::move(region));
	}

	template<particle_type ParticleGenType>
	inline void ParticleGenerator<ParticleGenType>::configure(std::string_view particle_type)
	{
		auto str = static_cast<std::string>(particle_type);
		if (!m_particles_instances.contains(str))
		{
			m_particles_instances.emplace(str, qs::ParticleInstance());
			m_particles_instances[str].load(m_texture.gl_texture(), m_texture_regions[str].m_width, m_texture_regions[str].m_height);
			m_particles_instances[str].create<qs::BufferDynamic>(m_texture_regions[str].m_x, m_texture_regions[str].m_y);

			std::vector<glm::vec2> temp(2);
			m_particles_instances[str].set_instance(temp);
		}
		else
		{
			PL_LOG(PL_WARNING, "Already configured particle of type: {0}.", str);
		}
	}

	template<particle_type ParticleGenType>
	inline void ParticleGenerator<ParticleGenType>::generate(std::string_view particle_type, const unsigned int amount, const float min_offset, const float max_offset, const float vel_x, const float vel_y)
	{
		m_amount           = amount;
		m_current_instance = static_cast<std::string>(particle_type);

		m_particles.clear();
		m_particles.reserve(m_amount);

		for (unsigned int count = 0; count < amount; count++)
		{
			m_particles.emplace_back();
			auto* particle = &m_particles.back();

			float x = 0.0f, y = 0.0f;

			if constexpr (std::is_same<ParticleGenType, HorizontalGen>::value)
			{
				x = pr::random<float>(min_offset, max_offset);
			}
			else if constexpr (std::is_same<ParticleGenType, VerticalGen>::value)
			{
				y = pr::random<float>(min_offset, max_offset);
			}
			else if constexpr (std::is_same<ParticleGenType, CircularGen>::value)
			{
				x = pr::random<float>(min_offset, max_offset);
				y = pr::random<float>(min_offset, max_offset);
			}

			particle->set_position(m_emitter_x + x, m_emitter_y + y);
			particle->set_velocity(vel_x, vel_y);
		}
	}

	template<particle_type ParticleGenType>
	inline void ParticleGenerator<ParticleGenType>::update(const double dt, const double life)
	{
		m_offsets.clear();
		m_offsets.reserve(m_amount);

		for (auto& particle : m_particles)
		{
			//particle.m_life -= life * dt;
			particle.move(dt);

			m_offsets.emplace_back(particle.pos());
		}

		m_particles_instances[m_current_instance].update_instances(m_offsets);
	}

	template<particle_type ParticleGenType>
	inline void ParticleGenerator<ParticleGenType>::bind()
	{
		m_particles_instances[m_current_instance].bind();
	}

	template<particle_type ParticleGenType>
	inline void ParticleGenerator<ParticleGenType>::unbind()
	{
		m_particles_instances[m_current_instance].unbind();
	}

	template<particle_type ParticleGenType>
	inline qs::ParticleInstance* ParticleGenerator<ParticleGenType>::get_instance()
	{
		return &m_particles_instances[m_current_instance];
	}

	template<particle_type ParticleGenType>
	inline const unsigned int ParticleGenerator<ParticleGenType>::amount() const noexcept
	{
		return m_amount;
	}

	template<particle_type ParticleGenType>
	inline const unsigned int ParticleGenerator<ParticleGenType>::gl_index_count()
	{
		return m_particles_instances[m_current_instance].index_count();
	}
} // namespace qs

#endif