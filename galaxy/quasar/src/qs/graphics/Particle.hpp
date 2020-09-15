///
/// Particle.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_PARTICLE_HPP_
#define QUASAR_PARTICLE_HPP_

#include "qs/texture/Texture.hpp"
#include "qs/core/Transform.hpp"
#include "qs/core/VertexData.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// Draws an instanced vertex array with a texture.
	///
	class Particle final : public VertexData, public Texture
	{
	public:
		///
		/// Constructor.
		///
		Particle();

		///
		/// Argument Constructor.
		///
		/// \param amount Amount of particles to draw.
		///
		Particle(unsigned int amount);

		///
		/// Copy constructor.
		///
		Particle(const Particle&) noexcept = delete;

		///
		/// Move constructor.
		///
		Particle(Particle&&) noexcept = default;

		///
		/// Copy assignment operator.
		///
		Particle& operator=(const Particle&) noexcept = delete;

		///
		/// Move assignment operator.
		///
		Particle& operator=(Particle&&) noexcept = default;

		///
		/// Destructor.
		///
		virtual ~Particle() noexcept = default;

		///
		/// \brief Creates the internal vertex array.
		///
		/// BufferType Fixed or dynamic buffer.
		///
		template<is_buffer BufferType>
		void create(std::span<glm::vec2> instances);

		///
		/// Set z-level of sprite.
		///
		/// \param z_level z-ordering level to render sprite at.
		///
		void set_z_level(const unsigned int z_level) noexcept;

		///
		/// Activate sprite context.
		///
		void bind() noexcept override;

		///
		/// Deactivate sprite context.
		///
		void unbind() noexcept override;

		///
		/// Get amount of particles to draw.
		///
		const unsigned int amount() const noexcept;

	private:
		///
		/// Amount of particles to draw.
		///
		unsigned int m_amount;

		///
		/// InstanceBuffer object.
		///
		qs::InstanceBuffer m_instance_buffer;
	};

	template<is_buffer BufferType>
	inline void Particle::create(std::span<glm::vec2> instances)
	{
		static_assert(std::is_same<BufferType, SpriteVertex>::value, "Particles must be a SpriteVertex BufferType.");

		auto v1 = make_vertex<SpriteVertex>(0.0f, 0.0f, 0.0f, 0.0f, 10.0f);
		auto v2 = make_vertex<SpriteVertex>(0.0f + m_width, 0.0f, 0.0f + m_width, 0.0f, 1.0f);
		auto v3 = make_vertex<SpriteVertex>(0.0f + m_width, 0.0f + m_height, 0.0f + m_width, 0.0f + m_height, 1.0f);
		auto v4 = make_vertex<SpriteVertex>(0.0f, 0.0f + m_height, 0.0f, 0.0f + m_height, 1.0f);

		m_vb.create<SpriteVertex, BufferType>({v1, v2, v3, v4});
		m_ib.create<BufferStatic>({0, 1, 3, 1, 2, 3});

		m_layout.add<SpriteVertex, VAPosition>(2);
		m_layout.add<SpriteVertex, VATexel>(2);

		m_va.create<SpriteVertex>(m_vb, m_ib, m_layout);

		m_instance_buffer.create(instances, 1);
		m_va.set_instanced(m_instance_buffer);
	}
} // namespace qs

#endif