///
/// Particle.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_PARTICLE_HPP_
#define QUASAR_PARTICLE_HPP_

#include "qs/core/Transform.hpp"
#include "qs/core/VertexData.hpp"
#include "qs/texture/Texture.hpp"

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
		/// Lifespan of particle.
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

	///
	/// Just an instanced vertex array with a texture.
	///
	class ParticleInstance final : public VertexData, public Texture
	{
	public:
		///
		/// Constructor.
		///
		ParticleInstance() = default;

		///
		/// Copy constructor.
		///
		ParticleInstance(const ParticleInstance&) = delete;

		///
		/// Move constructor.
		///
		ParticleInstance(ParticleInstance&&);

		///
		/// Copy assignment operator.
		///
		ParticleInstance& operator=(const ParticleInstance&) = delete;

		///
		/// Move assignment operator.
		///
		ParticleInstance& operator=(ParticleInstance&&);

		///
		/// Destructor.
		///
		virtual ~ParticleInstance() = default;

		///
		/// \brief Creates the internal vertex array.
		///
		/// BufferType Fixed or dynamic buffer.
		///
		/// \param tex_x Optional texture x pos.
		/// \param tex_y Optional texture y pos.
		///
		template<is_buffer BufferType>
		void create(const float tex_x = 0.0f, const float tex_y = 0.0f);

		///
		/// Set opacity.
		///
		/// Only affects the currently active quad.
		///
		/// \param opacity Opacity range is from 0.0f (transparent) to 1.0f (opaque).
		///
		void set_opacity(const float opacity) noexcept;

		///
		/// Creates the internal instancing vertex array.
		///
		/// \param instances Set of instance coords to spawn each particle at.
		///
		void set_instance(std::span<glm::vec2> instances);

		///
		/// Update instance vertex array.
		///
		/// \param instances Set of instance coords to spawn each particle at.
		///
		void update_instances(std::span<glm::vec2> instances);

		///
		/// Activate sprite context.
		///
		void bind() noexcept override;

		///
		/// Deactivate sprite context.
		///
		void unbind() noexcept override;

		///
		/// Get opacity.
		///
		/// \return Const float.
		///
		[[nodiscard]] const float opacity() const noexcept;

	private:
		///
		/// Opacity of sprite.
		///
		float m_opacity;

		///
		/// InstanceBuffer object.
		///
		qs::InstanceBuffer m_instance_buffer;
	};

	template<is_buffer BufferType>
	inline void ParticleInstance::create(const float tex_x, const float tex_y)
	{
		m_opacity = 1.0f;

		auto v1 = qs::make_vertex<qs::SpriteVertex>(0.0f, 0.0f, tex_x, tex_y, m_opacity);
		auto v2 = qs::make_vertex<qs::SpriteVertex>(0.0f + m_width, 0.0f, tex_x + m_width, tex_y, m_opacity);
		auto v3 = qs::make_vertex<qs::SpriteVertex>(0.0f + m_width, 0.0f + m_height, tex_x + m_width, tex_y + m_height, m_opacity);
		auto v4 = qs::make_vertex<qs::SpriteVertex>(0.0f, 0.0f + m_height, tex_x, tex_y + m_height, m_opacity);

		std::array<qs::SpriteVertex, 4> vb_arr = {v1, v2, v3, v4};
		std::array<unsigned int, 6> ib_arr     = {0, 1, 3, 1, 2, 3};
		m_vb.create<qs::SpriteVertex, BufferType>(vb_arr);
		m_ib.create<qs::BufferStatic>(ib_arr);

		m_layout.add<qs::SpriteVertex, qs::VAPosition>(2);
		m_layout.add<qs::SpriteVertex, qs::VATexel>(2);
		m_layout.add<qs::SpriteVertex, qs::VAOpacity>(1);

		m_va.create<qs::SpriteVertex>(m_vb, m_ib, m_layout);
	}
} // namespace qs

#endif