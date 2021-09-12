///
/// ParticleEffect.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more detailpe.
///

#include <execution>

#include <glm/trigonometric.hpp>

#include "galaxy/math/Random.hpp"

#include "ParticleEffect.hpp"

namespace galaxy
{
	namespace components
	{
		ParticleEffect::ParticleEffect() noexcept
			: Serializable {this}
			, m_opacity {255}
			, m_count {0}
			, m_radius {1.0f}
			, m_starting_pos {0.0f, 0.0f}
			, m_velocity {1.0f, 1.0f}
		{
		}

		ParticleEffect::ParticleEffect(const nlohmann::json& json)
			: Serializable {this}
			, m_opacity {255}
			, m_count {0}
			, m_radius {1.0f}
			, m_starting_pos {0.0f, 0.0f}
			, m_velocity {1.0f, 1.0f}
		{
			deserialize(json);
		}

		ParticleEffect::ParticleEffect(ParticleEffect&& pe) noexcept
			: Serializable {this}
			, Texture {std::move(pe)}
		{
			this->m_vao          = std::move(pe.m_vao);
			this->m_ibo          = std::move(pe.m_ibo);
			this->m_opacity      = pe.m_opacity;
			this->m_layer        = std::move(pe.m_layer);
			this->m_instances    = std::move(pe.m_instances);
			this->m_offsets      = std::move(pe.m_offsets);
			this->m_count        = pe.m_count;
			this->m_radius       = pe.m_radius;
			this->m_starting_pos = std::move(pe.m_starting_pos);
			this->m_velocity     = std::move(pe.m_velocity);
		}

		ParticleEffect& ParticleEffect::operator=(ParticleEffect&& pe) noexcept
		{
			if (this != &pe)
			{
				Texture::operator=(std::move(pe));

				this->m_vao          = std::move(pe.m_vao);
				this->m_ibo          = std::move(pe.m_ibo);
				this->m_opacity      = pe.m_opacity;
				this->m_layer        = std::move(pe.m_layer);
				this->m_instances    = std::move(pe.m_instances);
				this->m_offsets      = std::move(pe.m_offsets);
				this->m_count        = pe.m_count;
				this->m_radius       = pe.m_radius;
				this->m_starting_pos = std::move(pe.m_starting_pos);
				this->m_velocity     = std::move(pe.m_velocity);
			}

			return *this;
		}

		ParticleEffect::~ParticleEffect() noexcept
		{
			m_instances.clear();
			m_offsets.clear();
		}

		void ParticleEffect::create(std::string_view layer, const int count, const float radius, const glm::vec2& starting_pos, const glm::vec2& velocity)
		{
			m_layer        = static_cast<std::string>(layer);
			m_count        = count;
			m_radius       = radius;
			m_starting_pos = starting_pos;
			m_velocity     = velocity;

			std::array<graphics::Vertex, 4> vertices;
			vertices[0].m_pos    = {0.0f, 0.0f};
			vertices[0].m_texels = {0.0f, 0.0f};

			vertices[1].m_pos    = {0.0f + m_width, 0.0f};
			vertices[1].m_texels = {0.0f + m_width, 0.0f};

			vertices[2].m_pos    = {0.0f + m_width, 0.0f + m_height};
			vertices[2].m_texels = {0.0f + m_width, 0.0f + m_height};

			vertices[3].m_pos    = {0.0f, 0.0f + m_height};
			vertices[3].m_texels = {0.0f, 0.0f + m_height};

			std::array<unsigned int, 6> indices = {0, 1, 3, 1, 2, 3};

			graphics::VertexBuffer vbo;
			graphics::IndexBuffer ibo;

			vbo.create(vertices, false);
			ibo.create(indices, true);
			m_vao.create(vbo, ibo);

			// 70% - 130% of count.
			const auto random_count = static_cast<int>(m_count * math::random(0.7f, 1.3f));
			m_instances.resize(random_count);

			std::for_each(std::execution::par,
						  m_instances.begin(),
						  m_instances.end(),
						  [&](auto& particle)
						  {
							  const auto random_radius = m_radius * std::sqrt(math::random(0.0f, 1.0f));
							  const auto angle         = glm::radians(math::random(0.0f, 360.0f));
							  const auto random_start  = m_starting_pos + glm::vec2 {random_radius * glm::cos(angle), random_radius * glm::sin(angle)};
							  const auto random_vel    = m_velocity * math::random(0.7f, 1.3f);

							  particle.m_angle    = angle;
							  particle.m_offset   = random_start;
							  particle.m_velocity = random_vel;
							  particle.m_life     = 1.0f;
						  });

			buffer();
			m_vao.set_instanced(m_ibo);
		}

		void ParticleEffect::regen(std::optional<glm::vec2> new_pos)
		{
			m_instances.clear();

			if (new_pos != std::nullopt)
			{
				m_starting_pos = new_pos.value();
			}

			// 70% - 130% of count.
			const auto random_count = static_cast<int>(m_count * math::random(0.7f, 1.3f));
			m_instances.resize(random_count);

			std::for_each(std::execution::par,
						  m_instances.begin(),
						  m_instances.end(),
						  [&](auto& particle)
						  {
							  const auto random_radius = m_radius * std::sqrt(math::random(0.0f, 1.0f));
							  const auto angle         = glm::radians(math::random(0.0f, 360.0f));
							  const auto random_start  = m_starting_pos + glm::vec2 {random_radius * glm::cos(angle), random_radius * glm::sin(angle)};
							  const auto random_vel    = m_velocity * math::random(0.7f, 1.3f);

							  particle.m_angle    = angle;
							  particle.m_offset   = random_start;
							  particle.m_velocity = random_vel;
							  particle.m_life     = 1.0f;
						  });

			buffer();
		}

		void ParticleEffect::buffer()
		{
			m_offsets.clear();

			if (m_instances.size() > m_offsets.capacity())
			{
				m_offsets.reserve(m_instances.size());
			}

			for (const auto& particle : m_instances)
			{
				m_offsets.push_back(particle.m_offset);
			}

			m_ibo.create(m_offsets);
		}

		void ParticleEffect::bind() noexcept
		{
			m_vao.bind();
			glBindTexture(GL_TEXTURE_2D, m_texture);
		}

		void ParticleEffect::unbind() noexcept
		{
			m_vao.unbind();
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void ParticleEffect::set_opacity(const std::uint8_t opacity) noexcept
		{
			m_opacity = std::clamp<std::uint8_t>(opacity, 0, 255);
		}

		const std::string& ParticleEffect::get_layer() const noexcept
		{
			return m_layer;
		}

		const std::uint8_t ParticleEffect::get_opacity() const noexcept
		{
			return m_opacity;
		}

		std::vector<graphics::Particle>& ParticleEffect::get_particles() noexcept
		{
			return m_instances;
		}

		const unsigned int ParticleEffect::vao() const noexcept
		{
			return m_vao.id();
		}

		const int ParticleEffect::index_count() const noexcept
		{
			return m_vao.index_count();
		}

		const unsigned int ParticleEffect::instance_count() const noexcept
		{
			return m_ibo.instance_count();
		}

		nlohmann::json ParticleEffect::serialize()
		{
			nlohmann::json json = "{}"_json;

			json["texture"] = m_path;
			json["layer"]   = m_layer;
			json["opacity"] = m_opacity;
			json["count"]   = m_count;
			json["radius"]  = m_radius;

			json["starting-pos"]["x"] = m_starting_pos.x;
			json["starting-pos"]["y"] = m_starting_pos.y;
			json["velocity"]["x"]     = m_velocity.x;
			json["velocity"]["y"]     = m_velocity.y;

			return json;
		}

		void ParticleEffect::deserialize(const nlohmann::json& json)
		{
			m_instances.clear();
			m_offsets.clear();

			set_opacity(json.at("opacity"));
			load(json.at("texture").get<std::string>());

			const auto& sp  = json.at("starting-pos");
			const auto& vel = json.at("velocity");
			create(json.at("layer"), json.at("count"), json.at("radius"), {sp.at("x"), sp.at("y")}, {vel.at("x"), vel.at("y")});
		}
	} // namespace components
} // namespace galaxy