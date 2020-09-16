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
	struct HorizontalGen
	{
		HorizontalGen() = delete;
	};

	struct VerticalGen
	{
		VerticalGen() = delete;
	};

	struct CircularGen
	{
		CircularGen() = delete;
	};

	template<typename Type>
	concept particle_type = (std::is_same<Type, HorizontalGen>::value ||
				 std::is_same<Type, VerticalGen>::value ||
				 std::is_same<Type, CircularGen>::value);

	///
	///
	///
	template<particle_type ParticleGenType>
	class ParticleGenerator final
	{
	public:
		ParticleGenerator(std::string_view particle_sheet, qs::Shader* shader);
		~ParticleGenerator();

		ParticleGenerator(const ParticleGenerator&) = delete;
		ParticleGenerator(ParticleGenerator&&);
		ParticleGenerator& operator=(const ParticleGenerator&) = delete;
		ParticleGenerator& operator                            =(ParticleGenerator&&);

		void configure(std::string_view particle_type, const unsigned int amount, const unsigned int min_offset, const unsigned int max_offset);
		void define(std::string_view particle_type, pr::Rect<int>& region);

	private:
		ParticleGenerator() = default;

		void bind();
		void unbind();

		qs::Texture m_texture;

		qs::Shader* m_shader;

		std::vector<glm::vec2> m_offsets;

		std::unordered_map<std::string, Particle> m_particles;
		std::unordered_map<std::string, pr::Rect<int>> m_texture_regions;
	};

	template<particle_type ParticleGenType>
	ParticleGenerator<ParticleGenType>::ParticleGenerator(std::string_view particle_sheet, qs::Shader* shader)
	    : m_shader {shader}
	{
		m_texture.load(particle_sheet);
	}

	template<particle_type ParticleGenType>
	inline ParticleGenerator<ParticleGenType>::ParticleGenerator(ParticleGenerator&& pg)
	{
		this->m_texture         = std::move(pg.m_texture);
		this->m_shader          = pg.m_shader;
		this->m_texture_regions = std::move(pg.m_texture_regions);

		pg.m_shader = nullptr;
	}

	template<particle_type ParticleGenType>
	inline ParticleGenerator<ParticleGenType>& ParticleGenerator<ParticleGenType>::operator=(ParticleGenerator&& pg)
	{
		if (this != &pg)
		{
			this->m_texture         = std::move(pg.m_texture);
			this->m_shader          = pg.m_shader;
			this->m_texture_regions = std::move(pg.m_texture_regions);

			pg.m_shader = nullptr;
		}

		return *this;
	}

	template<particle_type ParticleGenType>
	inline ParticleGenerator<ParticleGenType>::~ParticleGenerator()
	{
		pg.m_shader = nullptr;
		m_texture_regions.clear();
	}

	template<particle_type ParticleGenType>
	void ParticleGenerator<ParticleGenType>::configure(std::string_view particle_type, const unsigned int amount, const unsigned int min_offset, const unsigned int max_offset)
	{
		for (unsigned int count = 0; count < amount; count++)
		{
			if constexpr (std::is_same<ParticleGenType, HorizontalGen>::value)
			{
				auto x = protostar::random<unsigned int>(min_offset, max_offset);
				m_offsets.push_back(x, 0);
			}
			else if constexpr (std::is_same<ParticleGenType, VerticalGen>::value)
			{
				auto y = protostar::random<unsigned int>(min_offset, max_offset);
				m_offsets.push_back(0, y);
			}
			else if constexpr (std::is_same<ParticleGenType, CircularGen>::value)
			{
				auto x = protostar::random<unsigned int>(min_offset, max_offset);
				auto y = protostar::random<unsigned int>(min_offset, max_offset);
				m_offsets.push_back(x, y);
			}
		}

		m_particles.emplace(particle_type);
		m_particles[particle_type].create<SpriteVertex>();
	}

	template<particle_type ParticleGenType>
	void ParticleGenerator<ParticleGenType>::define(std::string_view particle_type, pr::Rect<int>& region)
	{
		m_texture_regions.emplace(static_cast<std::string>(particle_type), std::move(region));
	}

	template<particle_type ParticleGenType>
	void ParticleGenerator<ParticleGenType>::bind()
	{
	}

	template<particle_type ParticleGenType>
	void ParticleGenerator<ParticleGenType>::unbind()
	{
	}
} // namespace qs

#endif