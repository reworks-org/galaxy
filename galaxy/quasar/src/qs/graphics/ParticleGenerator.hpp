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

#include "qs/core/Shader.hpp"
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
		ParticleGenerator() = default;

		///
		/// Destructor.
		///
		~ParticleGenerator();

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
		/// Create particle generator.
		///
		/// \param particle_sheet Texture spritesheet containing particles for generator to use.
		///
		void create(std::string_view particle_sheet);

		///
		/// Define a particle type on the texture spritesheet.
		///
		/// \param particle_type Identifier for this texture region.
		/// \param region Texture coords on the spritesheet.
		///
		void define(std::string_view particle_type, pr::Rect<int> region);

		///
		/// Configure the generator with what type of particle to emit, amount, and the min/max offset of the instancing random pos generator.
		///
		/// \param particle_type
		/// \param amount
		/// \param min_offset
		/// \param max_offset
		///
		void configure(std::string_view particle_type, const unsigned int amount, const unsigned int min_offset, const unsigned int max_offset);

		///
		/// Get the current amount of particles being drawn.
		///
		/// \return Const unsigned integer.
		///
		const unsigned int amount() const noexcept;

		///
		/// Get the current OpenGL index element buffer count.
		///
		/// \return Const unsigned integer.
		///
		const unsigned int gl_index_count();

		///
		/// Bind particle generator to active opengl instance.
		///
		void bind();

		///
		/// Unbind particle generator from active opengl instance.
		///
		void unbind();

	private:
		///
		/// Amount of particles being generated per instance.
		///
		unsigned int m_amount;

		///
		/// Currently active particle.
		///
		std::string m_current;

		///
		/// Particle generator sprite sheet.
		///
		qs::Texture m_texture;

		///
		/// Offsets of currently configured particles.
		///
		std::vector<glm::vec2> m_offsets;

		///
		/// Array of different particles this generator can produce.
		///
		std::unordered_map<std::string, Particle> m_particles;

		///
		/// Region of each particle type.
		///
		std::unordered_map<std::string, pr::Rect<int>> m_texture_regions;
	};

	template<particle_type ParticleGenType>
	inline void ParticleGenerator<ParticleGenType>::create(std::string_view particle_sheet)
	{
		m_texture.load(particle_sheet);
	}

	template<particle_type ParticleGenType>
	inline ParticleGenerator<ParticleGenType>::ParticleGenerator(ParticleGenerator&& pg)
	{
		this->m_texture         = std::move(pg.m_texture);
		this->m_texture_regions = std::move(pg.m_texture_regions);
	}

	template<particle_type ParticleGenType>
	inline ParticleGenerator<ParticleGenType>& ParticleGenerator<ParticleGenType>::operator=(ParticleGenerator&& pg)
	{
		if (this != &pg)
		{
			this->m_texture         = std::move(pg.m_texture);
			this->m_texture_regions = std::move(pg.m_texture_regions);
		}

		return *this;
	}

	template<particle_type ParticleGenType>
	inline ParticleGenerator<ParticleGenType>::~ParticleGenerator()
	{
		m_texture_regions.clear();
	}

	template<particle_type ParticleGenType>
	void ParticleGenerator<ParticleGenType>::define(std::string_view particle_type, pr::Rect<int> region)
	{
		m_texture_regions.emplace(static_cast<std::string>(particle_type), std::move(region));
	}

	template<particle_type ParticleGenType>
	void ParticleGenerator<ParticleGenType>::configure(std::string_view particle_type, const unsigned int amount, const unsigned int min_offset, const unsigned int max_offset)
	{
		m_amount = amount;
		m_offsets.clear();

		for (unsigned int count = 0; count < amount; count++)
		{
			if constexpr (std::is_same<ParticleGenType, HorizontalGen>::value)
			{
				auto x = pr::random<unsigned int>(min_offset, max_offset);
				m_offsets.push_back(x, 0);
			}
			else if constexpr (std::is_same<ParticleGenType, VerticalGen>::value)
			{
				auto y = pr::random<unsigned int>(min_offset, max_offset);
				m_offsets.push_back(0, y);
			}
			else if constexpr (std::is_same<ParticleGenType, CircularGen>::value)
			{
				auto x = pr::random<unsigned int>(min_offset, max_offset);
				auto y = pr::random<unsigned int>(min_offset, max_offset);
				m_offsets.push_back({x, y});
			}
		}

		m_current = static_cast<std::string>(particle_type);
		if (!m_particles.contains(m_current))
		{
			m_particles.emplace(m_current, qs::Particle());
		}

		m_particles[m_current].load(m_texture.gl_texture(), m_texture_regions[m_current].m_width, m_texture_regions[m_current].m_height);
		m_particles[m_current].create<qs::BufferStatic>(m_texture_regions[m_current].m_x, m_texture_regions[m_current].m_y);
		m_particles[m_current].set_instance(m_offsets);
	}

	template<particle_type ParticleGenType>
	inline const unsigned int ParticleGenerator<ParticleGenType>::amount() const noexcept
	{
		return m_amount;
	}

	template<particle_type ParticleGenType>
	inline const unsigned int ParticleGenerator<ParticleGenType>::gl_index_count()
	{
		return m_particles[m_current].index_count();
	}

	template<particle_type ParticleGenType>
	void ParticleGenerator<ParticleGenType>::bind()
	{
		m_texture.bind();
		m_particles[m_current].bind();
	}

	template<particle_type ParticleGenType>
	void ParticleGenerator<ParticleGenType>::unbind()
	{
		m_texture.unbind();
	}
} // namespace qs

#endif