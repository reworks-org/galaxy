///
/// ParticleGenerator.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glm/geometric.hpp>
#include <nlohmann/json.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/graphics/Primitives.hpp"
#include "galaxy/math/Random.hpp"
#include "galaxy/platform/Pragma.hpp"
#include "galaxy/resource/TextureAtlas.hpp"

#include "ParticleGenerator.hpp"

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_PUSH
GALAXY_DISABLE_WARNING(26477)
#endif

namespace galaxy
{
	namespace components
	{
		ParticleGenerator::ParticleGenerator()
			: Serializable {}
			, m_count {0}
			, m_randomize_position {true}
			, m_randomize_initial_vel {true}
			, m_randomize_life {true}
			, m_randomize_scale {true}
			, m_keep_scale_aspect_ratio {true}
			, m_randomize_colour {true}
			, m_randomize_colour_alpha {true}
			, m_spread {graphics::ParticleSpread::SPREAD_TYPE_SPHERE}
			, m_spread_radius {2.0f}
			, m_min_rect_spread {-3.0f, -2.0f}
			, m_max_rect_spread {3.0f, 2.0f}
			, m_fixed_vel {0.0f, 0.0f}
			, m_min_vel {-1.0f, 1.0f}
			, m_max_vel {1.0f, 5.0f}
			, m_fixed_life {1.0f}
			, m_min_life {0.1f}
			, m_max_life {5.0f}
			, m_fixed_scale {0.1f, 0.1f}
			, m_min_scale {0.1f, 0.1f}
			, m_max_scale {0.2f, 0.2f}
			, m_fixed_colour {1.0f, 1.0f, 1.0f}
			, m_min_colour {0.0f, 0.0f, 0.0f}
			, m_max_colour {1.0f, 1.0f, 1.0f}
			, m_fixed_alpha {1.0f}
			, m_min_alpha {0.0f}
			, m_max_alpha {1.0f}
			, m_instance {0}
			, m_vbo {0}
			, m_ebo {0}
			, m_vao {0}
			, m_tex_id {0}
			, m_layer {0}
		{
		}

		ParticleGenerator::ParticleGenerator(const nlohmann::json& json)
			: Serializable {}
			, m_count {0}
			, m_instance {0}
			, m_vbo {0}
			, m_ebo {0}
			, m_vao {0}
			, m_tex_id {0}
			, m_layer {0}
		{
			deserialize(json);
		}

		ParticleGenerator::ParticleGenerator(ParticleGenerator&& p)
			: Serializable {}
			, m_count {0}
			, m_instance {0}
			, m_vbo {0}
			, m_ebo {0}
			, m_vao {0}
			, m_tex_id {0}
			, m_layer {0}
		{
			this->m_count                   = p.m_count;
			this->m_randomize_position      = p.m_randomize_position;
			this->m_randomize_initial_vel   = p.m_randomize_initial_vel;
			this->m_randomize_life          = p.m_randomize_life;
			this->m_randomize_scale         = p.m_randomize_scale;
			this->m_keep_scale_aspect_ratio = p.m_keep_scale_aspect_ratio;
			this->m_randomize_colour        = p.m_randomize_colour;
			this->m_randomize_colour_alpha  = p.m_randomize_colour_alpha;
			this->m_spread                  = p.m_spread;
			this->m_spread_radius           = p.m_spread_radius;
			this->m_min_rect_spread         = std::move(p.m_min_rect_spread);
			this->m_max_rect_spread         = std::move(p.m_max_rect_spread);
			this->m_fixed_vel               = std::move(p.m_fixed_vel);
			this->m_min_vel                 = std::move(p.m_min_vel);
			this->m_max_vel                 = std::move(p.m_max_vel);
			this->m_fixed_life              = p.m_fixed_life;
			this->m_min_life                = p.m_min_life;
			this->m_max_life                = p.m_max_life;
			this->m_fixed_scale             = std::move(p.m_fixed_scale);
			this->m_min_scale               = std::move(p.m_min_scale);
			this->m_max_scale               = std::move(p.m_max_scale);
			this->m_fixed_colour            = std::move(p.m_fixed_colour);
			this->m_min_colour              = std::move(p.m_min_colour);
			this->m_max_colour              = std::move(p.m_max_colour);
			this->m_fixed_alpha             = p.m_fixed_alpha;
			this->m_min_alpha               = p.m_min_alpha;
			this->m_max_alpha               = p.m_max_alpha;
			this->m_layer                   = p.m_layer;

			this->m_instance  = p.m_instance;
			this->m_vao       = p.m_vao;
			this->m_ebo       = p.m_ebo;
			this->m_vbo       = p.m_vbo;
			this->m_texture   = std::move(p.m_texture);
			this->m_particles = std::move(p.m_particles);
			this->m_tex_id    = p.m_tex_id;

			p.m_instance = 0;
			p.m_vao      = 0;
			p.m_vbo      = 0;
			p.m_ebo      = 0;
			p.m_tex_id   = 0;
		}

		ParticleGenerator& ParticleGenerator::operator=(ParticleGenerator&& p)
		{
			if (this != &p)
			{
				this->m_count                   = p.m_count;
				this->m_randomize_position      = p.m_randomize_position;
				this->m_randomize_initial_vel   = p.m_randomize_initial_vel;
				this->m_randomize_life          = p.m_randomize_life;
				this->m_randomize_scale         = p.m_randomize_scale;
				this->m_keep_scale_aspect_ratio = p.m_keep_scale_aspect_ratio;
				this->m_randomize_colour        = p.m_randomize_colour;
				this->m_randomize_colour_alpha  = p.m_randomize_colour_alpha;
				this->m_spread                  = p.m_spread;
				this->m_spread_radius           = p.m_spread_radius;
				this->m_min_rect_spread         = std::move(p.m_min_rect_spread);
				this->m_max_rect_spread         = std::move(p.m_max_rect_spread);
				this->m_fixed_vel               = std::move(p.m_fixed_vel);
				this->m_min_vel                 = std::move(p.m_min_vel);
				this->m_max_vel                 = std::move(p.m_max_vel);
				this->m_fixed_life              = p.m_fixed_life;
				this->m_min_life                = p.m_min_life;
				this->m_max_life                = p.m_max_life;
				this->m_fixed_scale             = std::move(p.m_fixed_scale);
				this->m_min_scale               = std::move(p.m_min_scale);
				this->m_max_scale               = std::move(p.m_max_scale);
				this->m_fixed_colour            = std::move(p.m_fixed_colour);
				this->m_min_colour              = std::move(p.m_min_colour);
				this->m_max_colour              = std::move(p.m_max_colour);
				this->m_fixed_alpha             = p.m_fixed_alpha;
				this->m_min_alpha               = p.m_min_alpha;
				this->m_max_alpha               = p.m_max_alpha;
				this->m_layer                   = p.m_layer;

				this->m_instance  = p.m_instance;
				this->m_vao       = p.m_vao;
				this->m_vbo       = p.m_vbo;
				this->m_ebo       = p.m_ebo;
				this->m_texture   = std::move(p.m_texture);
				this->m_particles = std::move(p.m_particles);
				this->m_tex_id    = p.m_tex_id;

				p.m_instance = 0;
				p.m_vao      = 0;
				p.m_vbo      = 0;
				p.m_ebo      = 0;
				p.m_tex_id   = 0;
			}

			return *this;
		}

		ParticleGenerator::~ParticleGenerator()
		{
			if (m_instance != 0)
			{
				glDeleteBuffers(1, &m_instance);
			}

			if (m_vao != 0)
			{
				glDeleteVertexArrays(1, &m_vao);
			}

			if (m_vbo != 0)
			{
				glDeleteBuffers(1, &m_vbo);
			}

			if (m_ebo != 0)
			{
				glDeleteBuffers(1, &m_ebo);
			}
		}

		void ParticleGenerator::generate(const glm::vec2& pos, const std::string& texture, const int layer, const int count)
		{
			auto&      atlas    = core::ServiceLocator<resource::TextureAtlas>::ref();
			const auto info_opt = atlas.query(texture);
			if (info_opt.has_value())
			{
				const auto& info = info_opt.value().get();

				m_start_pos = pos;

				m_texture = texture;
				m_tex_id  = info.m_handle;

				m_layer = layer;
				m_count = count;

				m_particles.clear();
				m_particles.resize(m_count);

				reset();
				generate_buffers();
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to generate particles with texture '{0}'.");
			}
		}

		void ParticleGenerator::regenerate()
		{
			m_particles.clear();
			m_particles.resize(m_count);

			reset();
			generate_buffers();
		}

		void ParticleGenerator::reset()
		{
			for (auto i = 0; i < m_count; i++)
			{
				reset(i);
			}
		}

		void ParticleGenerator::buffer_instances()
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_instance);
			glBufferData(GL_ARRAY_BUFFER, sizeof(graphics::Particle) * m_particles.size(), m_particles.data(), GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		int ParticleGenerator::get_instances() const
		{
			return m_count;
		}

		unsigned int ParticleGenerator::get_mode() const
		{
			return graphics::primitive_to_gl(graphics::Primitives::TRIANGLE);
		}

		unsigned int ParticleGenerator::get_vao() const
		{
			return m_vao;
		}

		unsigned int ParticleGenerator::get_texture() const
		{
			return m_tex_id;
		}

		unsigned int ParticleGenerator::get_count() const
		{
			return 6;
		}

		int ParticleGenerator::get_layer() const
		{
			return m_layer;
		}

		nlohmann::json ParticleGenerator::serialize()
		{
			nlohmann::json json             = "{}"_json;
			json["texture"]                 = m_texture;
			json["layer"]                   = m_layer;
			json["start"]["x"]              = m_start_pos.x;
			json["start"]["y"]              = m_start_pos.y;
			json["count"]                   = m_count;
			json["randomize_position"]      = m_randomize_position;
			json["randomize_initial_vel"]   = m_randomize_initial_vel;
			json["randomize_life"]          = m_randomize_life;
			json["randomize_scale"]         = m_randomize_scale;
			json["keep_scale_aspect_ratio"] = m_keep_scale_aspect_ratio;
			json["randomize_colour"]        = m_randomize_colour;
			json["randomize_colour_alpha"]  = m_randomize_colour_alpha;
			json["spread_radius;"]          = m_spread_radius;
			json["fixed_life"]              = m_fixed_life;
			json["min_life"]                = m_min_life;
			json["max_life"]                = m_max_life;
			json["fixed_alpha"]             = m_fixed_alpha;
			json["min_alpha"]               = m_min_alpha;
			json["max_alpha"]               = m_max_alpha;
			json["spread"]                  = magic_enum::enum_name(m_spread);
			json["min_rect_spread"]["x"]    = m_min_rect_spread.x;
			json["min_rect_spread"]["y"]    = m_min_rect_spread.y;
			json["max_rect_spread"]["x"]    = m_max_rect_spread.x;
			json["max_rect_spread"]["y"]    = m_max_rect_spread.y;
			json["fixed_vel"]["x"]          = m_fixed_vel.x;
			json["fixed_vel"]["y"]          = m_fixed_vel.y;
			json["min_vel"]["x"]            = m_min_vel.x;
			json["min_vel"]["y"]            = m_min_vel.y;
			json["max_vel"]["x"]            = m_max_vel.x;
			json["max_vel"]["y"]            = m_max_vel.y;
			json["fixed_scale"]["x"]        = m_fixed_scale.x;
			json["fixed_scale"]["y"]        = m_fixed_scale.y;
			json["min_scale"]["x"]          = m_min_scale.x;
			json["min_scale"]["y"]          = m_min_scale.y;
			json["max_scale"]["x"]          = m_max_scale.x;
			json["max_scale"]["y"]          = m_max_scale.y;
			json["fixed_colour"]["x"]       = m_fixed_colour.x;
			json["fixed_colour"]["y"]       = m_fixed_colour.y;
			json["fixed_colour"]["z"]       = m_fixed_colour.z;
			json["min_colour"]["x"]         = m_min_colour.x;
			json["min_colour"]["y"]         = m_min_colour.y;
			json["min_colour"]["z"]         = m_min_colour.z;
			json["max_colour"]["x"]         = m_max_colour.x;
			json["max_colour"]["y"]         = m_max_colour.y;
			json["max_colour"]["z"]         = m_max_colour.z;

			return json;
		}

		void ParticleGenerator::deserialize(const nlohmann::json& json)
		{
			m_texture = json.at("texture");
			m_layer   = json.at("layer");

			const auto& start_pos = json.at("start_pos");
			m_start_pos.x         = start_pos.at("x");
			m_start_pos.y         = start_pos.at("y");

			m_count                   = json.at("count");
			m_randomize_position      = json.at("randomize_position");
			m_randomize_initial_vel   = json.at("randomize_initial_vel");
			m_randomize_life          = json.at("randomize_life");
			m_randomize_scale         = json.at("randomize_scale");
			m_keep_scale_aspect_ratio = json.at("keep_scale_aspect_ratio");
			m_randomize_colour        = json.at("randomize_colour");
			m_randomize_colour_alpha  = json.at("randomize_colour_alpha");
			m_spread_radius           = json.at("spread_radius");
			m_fixed_life              = json.at("fixed_life");
			m_min_life                = json.at("min_life");
			m_max_life                = json.at("max_life");
			m_fixed_alpha             = json.at("fixed_alpha");
			m_min_alpha               = json.at("min_alpha");
			m_max_alpha               = json.at("max_alpha");

			std::string spread = json.at("spread");
			auto        opt    = magic_enum::enum_cast<graphics::ParticleSpread>(spread);
			if (opt.has_value())
			{
				m_spread = opt.value();
			}

			const auto& min_rect_spread = json.at("min_rect_spread");
			m_min_rect_spread.x         = min_rect_spread.at("x");
			m_min_rect_spread.y         = min_rect_spread.at("y");

			const auto& max_rect_spread = json.at("max_rect_spread");
			m_max_rect_spread.x         = max_rect_spread.at("x");
			m_max_rect_spread.y         = max_rect_spread.at("y");

			const auto& fixed_vel = json.at("fixed_vel");
			m_fixed_vel.x         = fixed_vel.at("x");
			m_fixed_vel.y         = fixed_vel.at("y");

			const auto& min_vel = json.at("min_vel");
			m_min_vel.x         = min_vel.at("x");
			m_min_vel.y         = min_vel.at("y");

			const auto& max_vel = json.at("max_vel");
			m_max_vel.x         = max_vel.at("x");
			m_max_vel.y         = max_vel.at("y");

			const auto& fixed_scale = json.at("fixed_scale");
			m_fixed_scale.x         = fixed_scale.at("x");
			m_fixed_scale.y         = fixed_scale.at("y");

			const auto& min_scale = json.at("min_scale");
			m_min_scale.x         = min_scale.at("x");
			m_min_scale.y         = min_scale.at("y");

			const auto& max_scale = json.at("max_scale");
			m_max_scale.x         = max_scale.at("x");
			m_max_scale.y         = max_scale.at("y");

			const auto& fixed_colour = json.at("fixed_colour");
			m_fixed_colour.x         = fixed_colour.at("x");
			m_fixed_colour.y         = fixed_colour.at("y");
			m_fixed_colour.z         = fixed_colour.at("z");

			const auto& min_colour = json.at("min_colour");
			m_min_colour.x         = min_colour.at("x");
			m_min_colour.y         = min_colour.at("y");
			m_min_colour.z         = min_colour.at("z");

			const auto& max_colour = json.at("max_colour");
			m_max_colour.x         = max_colour.at("x");
			m_max_colour.y         = max_colour.at("y");
			m_max_colour.z         = max_colour.at("z");

			regenerate();
		}

		void ParticleGenerator::reset(const unsigned int index)
		{
			math::Random random;

			if (m_randomize_position)
			{
				switch (m_spread)
				{
					case graphics::ParticleSpread::SPREAD_TYPE_SPHERE:
						{
							const auto vec = random.gen_vec3({-1, -1, -1}, {1, 1, 1});
							const auto nor = glm::normalize(vec) * random.gen<float>(0.0f, m_spread_radius);

							m_particles[index].m_pos = {nor.x + m_start_pos.x, nor.y + m_start_pos.y};
						}
						break;
					case graphics::ParticleSpread::SPREAD_TYPE_RECTANGLE:
						m_particles[index].m_pos = random.gen_vec2(m_min_rect_spread, m_max_rect_spread) + m_start_pos;
						break;
				}
			}
			else
			{
				m_particles[index].m_pos = m_start_pos;
			}

			if (m_randomize_initial_vel)
			{
				m_particles[index].m_vel = random.gen_vec2(m_min_vel, m_max_vel);
			}
			else
			{
				m_particles[index].m_vel = m_fixed_vel;
			}

			if (m_randomize_life)
			{
				m_particles[index].m_life = random.gen<float>(m_min_life, m_max_life);
			}
			else
			{
				m_particles[index].m_life = m_fixed_life;
			}

			if (m_randomize_scale)
			{
				if (m_keep_scale_aspect_ratio)
				{
					const auto scale           = random.gen<float>(m_min_scale.x, m_max_scale.x);
					m_particles[index].m_scale = {scale, scale};
				}
				else
				{
					m_particles[index].m_scale = random.gen_vec2(m_min_scale, m_max_scale);
				}
			}
			else
			{
				m_particles[index].m_scale = m_fixed_scale;
			}

			if (m_randomize_colour)
			{
				if (m_randomize_colour_alpha)
				{
					const glm::vec3 col   = {random.gen<float>(m_min_colour.x, m_max_colour.x),
						  random.gen<float>(m_min_colour.y, m_max_colour.y),
						  random.gen<float>(m_min_colour.z, m_max_colour.z)};
					const auto      alpha = random.gen<float>(m_min_alpha, m_max_alpha);

					m_particles[index].m_colour = {col, alpha};
				}
				else
				{
					const glm::vec3 col = {random.gen<float>(m_min_colour.x, m_max_colour.x),
						random.gen<float>(m_min_colour.y, m_max_colour.y),
						random.gen<float>(m_min_colour.z, m_max_colour.z)};

					m_particles[index].m_colour = {col, m_fixed_alpha};
				}
			}
			else
			{
				if (m_randomize_colour_alpha)
				{
					m_particles[index].m_colour = {m_fixed_colour, random.gen<float>(m_min_alpha, m_max_alpha)};
				}
				else
				{
					m_particles[index].m_colour = {m_fixed_colour, m_fixed_alpha};
				}
			}
		}

		void ParticleGenerator::generate_buffers()
		{
			// clang-format off
			static constexpr const std::array<float, 16> vertices = {
				0.0f, 0.0f, 0.0f, 0.0f, // TL, xyuv
				1.0f, 0.0f,1.0f, 0.0f, // TR, xyuv
				1.0f, 1.0f, 1.0f, 1.0f, // BL, xyuv
				0.0f, 1.0f, 0.0f, 1.0f // BR, xyuv
			};

			static constexpr const std::array<unsigned int, 6> indices = {
                0u, 1u, 3u, 1u, 2u, 3u
			};
			// clang-format on

			constexpr const auto particle_size = sizeof(graphics::Particle);

			if (m_instance != 0)
			{
				glDeleteBuffers(1, &m_instance);
			}

			if (m_vao != 0)
			{
				glDeleteVertexArrays(1, &m_vao);
			}

			if (m_ebo != 0)
			{
				glDeleteBuffers(1, &m_ebo);
			}

			if (m_vbo != 0)
			{
				glDeleteBuffers(1, &m_vbo);
			}

			glGenBuffers(1, &m_instance);
			glBindBuffer(GL_ARRAY_BUFFER, m_instance);
			glBufferData(GL_ARRAY_BUFFER, particle_size * m_particles.size(), m_particles.data(), GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glGenVertexArrays(1, &m_vao);
			glGenBuffers(1, &m_vbo);
			glGenBuffers(1, &m_ebo);
			glBindVertexArray(m_vao);

			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * vertices.size(), indices.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
			glEnableVertexAttribArray(0);

			glBindBuffer(GL_ARRAY_BUFFER, m_instance);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, particle_size, reinterpret_cast<void*>(offsetof(graphics::Particle, m_pos)));
			glEnableVertexAttribArray(1);

			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, particle_size, reinterpret_cast<void*>(offsetof(graphics::Particle, m_scale)));
			glEnableVertexAttribArray(2);

			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, particle_size, reinterpret_cast<void*>(offsetof(graphics::Particle, m_colour)));
			glEnableVertexAttribArray(3);

			glVertexAttribDivisor(1, 1);
			glVertexAttribDivisor(2, 1);
			glVertexAttribDivisor(3, 1);

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	} // namespace components
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_POP
#endif
