///
/// ParticleGenerator.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>

#include "galaxy/algorithm/Random.hpp"

#include "ParticleGenerator.hpp"

namespace galaxy
{
	namespace graphics
	{
		ParticleGenerator::ParticleGenerator()
		    : m_emitter_x {0.0f}, m_emitter_y {0.0f}, m_amount {0}, m_finished {true}, m_current_instance {""}, m_current_particle {0}
		{
		}

		ParticleGenerator::ParticleGenerator(ParticleGenerator&& pg)
		{
			this->m_emitter_x           = pg.m_emitter_x;
			this->m_emitter_y           = pg.m_emitter_y;
			this->m_amount              = pg.m_amount;
			this->m_finished            = pg.m_finished;
			this->m_current_instance    = pg.m_current_instance;
			this->m_texture             = std::move(pg.m_texture);
			this->m_current_particle    = pg.m_current_particle;
			this->m_offsets             = std::move(pg.m_offsets);
			this->m_particles           = std::move(pg.m_particles);
			this->m_particles_instances = std::move(pg.m_particles_instances);
			this->m_texture_regions     = std::move(pg.m_texture_regions);
		}

		ParticleGenerator& ParticleGenerator::operator=(ParticleGenerator&& pg)
		{
			if (this != &pg)
			{
				this->m_emitter_x           = pg.m_emitter_x;
				this->m_emitter_y           = pg.m_emitter_y;
				this->m_amount              = pg.m_amount;
				this->m_finished            = pg.m_finished;
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

		ParticleGenerator::~ParticleGenerator()
		{
			m_offsets.clear();
			m_particles.clear();
			m_particles_instances.clear();
			m_texture_regions.clear();
		}

		void ParticleGenerator::create(std::string_view particle_sheet, float emitter_x, float emitter_y)
		{
			m_emitter_x = emitter_x;
			m_emitter_y = emitter_y;
			m_texture.load(particle_sheet);
		}

		void ParticleGenerator::define(std::string_view particle_type, graphics::Rect<int> region)
		{
			m_texture_regions.emplace(static_cast<std::string>(particle_type), std::move(region));
		}

		void ParticleGenerator::configure(std::string_view particle_type)
		{
			auto str = static_cast<std::string>(particle_type);
			if (!m_particles_instances.contains(str))
			{
				m_particles_instances.emplace(str, ParticleInstance());
				m_particles_instances[str].load(m_texture.gl_texture(), m_texture_regions[str].m_width, m_texture_regions[str].m_height);
				m_particles_instances[str].create<BufferDynamic>(m_texture_regions[str].m_x, m_texture_regions[str].m_y);
				m_particles_instances[str].set_instance({});
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Already configured particle of type: {0}.", str);
			}
		}

		void ParticleGenerator::gen_linear(std::string_view particle_type, const unsigned int amount, const float max_offset_x, const float max_offset_y, float vel_x, float vel_y)
		{
			m_amount           = amount;
			m_current_instance = static_cast<std::string>(particle_type);

			m_particles.clear();
			m_particles.reserve(m_amount);

			for (unsigned int count = 0; count < amount; count++)
			{
				Particle particle;

				auto x = algorithm::random<float>(0.0f, max_offset_x);
				auto y = algorithm::random<float>(0.0f, max_offset_y);

				particle.set_position(m_emitter_x + x, m_emitter_y + y);
				particle.set_velocity(vel_x, vel_y);
				m_particles.push_back(particle);
			}

			m_finished = false;
		}

		void ParticleGenerator::gen_circular(std::string_view particle_type, const unsigned int amount, const float radius, float vel_x, float vel_y)
		{
			m_amount           = amount;
			m_current_instance = static_cast<std::string>(particle_type);

			m_particles.clear();
			m_particles.reserve(m_amount);

			for (unsigned int count = 0; count < m_amount; count++)
			{
				Particle particle;

				const float random_radius = radius * std::sqrt(algorithm::random<float>(0.0f, 1.0f));
				const float angle         = algorithm::random<float>(0.0f, 1.0f) * 2.0f * glm::pi<float>();
				const float x             = m_emitter_x + random_radius * std::cos(angle);
				const float y             = m_emitter_y + random_radius * std::sin(angle);

				particle.set_position(x, y);
				particle.set_velocity(vel_x, vel_y);
				m_particles.push_back(particle);
			}

			m_finished = false;
		}

		void ParticleGenerator::update(const double dt)
		{
			m_offsets.clear();
			m_offsets.reserve(m_amount);

			if (!m_finished)
			{
				for (auto particle = m_particles.begin(); particle != m_particles.end();)
				{
					particle->m_life -= algorithm::random(0.01, 0.05) * dt;
					if (particle->m_life < 0.0f)
					{
						particle = m_particles.erase(particle);
					}
					else
					{
						particle->move(static_cast<float>(dt));
						m_offsets.emplace_back(particle->pos().x, particle->pos().y, particle->m_life);

						++particle;
					}
				}

				if (!m_particles.empty())
				{
					m_particles_instances[m_current_instance].update_instances(m_offsets);
				}
				else
				{
					m_finished = true;
				}
			}
		}

		void ParticleGenerator::update_emitter(const float emitter_x, const float emitter_y)
		{
			m_emitter_x = emitter_x;
			m_emitter_y = emitter_y;
		}

		void ParticleGenerator::bind()
		{
			m_particles_instances[m_current_instance].bind();
		}

		void ParticleGenerator::unbind()
		{
			m_particles_instances[m_current_instance].unbind();
		}

		ParticleInstance* ParticleGenerator::get_instance()
		{
			return &m_particles_instances[m_current_instance];
		}

		const bool ParticleGenerator::finished() const
		{
			return m_finished;
		}

		const unsigned int ParticleGenerator::amount() const
		{
			return m_amount;
		}

		const unsigned int ParticleGenerator::gl_index_count()
		{
			return m_particles_instances[m_current_instance].index_count();
		}
	} // namespace graphics
} // namespace galaxy