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
		Particle(pr::positive_integer auto amount);

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
		void create();

		///
		/// Set z-level of sprite.
		///
		/// \param z_level z-ordering level to render sprite at.
		///
		void set_z_level(const pr::positive_integer auto z_level) noexcept;

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
	};

	template<is_buffer BufferType>
	inline void Particle::create()
	{
		auto v1 = make_vertex<InstanceVertex>(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		auto v2 = make_vertex<InstanceVertex>(0.0f + m_width, 0.0f, 0.0f + m_width, 0.0f, 0.0f + m_width, 0.0f);
		auto v3 = make_vertex<InstanceVertex>(0.0f + m_width, 0.0f + m_height, 0.0f + m_width, 0.0f + m_height, 0.0f + m_width, 0.0f + m_height);
		auto v4 = make_vertex<InstanceVertex>(0.0f, 0.0f + m_height, 0.0f, 0.0f + m_height, 0.0f, 0.0f + m_height);

		m_vb.create<InstanceVertex, BufferType>({ v1, v2, v3, v4 });
		m_ib.create<BufferStatic>({ 0, 1, 3, 1, 2, 3 });

		m_layout.add<InstanceVertex, VAPosition>(2);
		m_layout.add<InstanceVertex, VATexel>(2);
		m_layout.add<InstanceVertex, VAInstanceOffset>(2);

		m_va.create<InstanceVertex>(m_vb, m_ib, m_layout);
		m_va.change_divisor(2, 1);
	}
} // namespace qs

#endif