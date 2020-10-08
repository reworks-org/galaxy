///
/// ParticleInstance.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_PARTICLEINSTANCE_HPP_
#define QUASAR_PARTICLEINSTANCE_HPP_

#include "qs/core/VertexData.hpp"
#include "qs/texture/Texture.hpp"

///
/// Core namespace.
///
namespace qs
{
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
		/// Creates the internal instancing vertex array.
		///
		/// \param instances Set of instance coords to spawn each particle at. Z is opacity.
		///
		void set_instance(std::span<glm::vec3> instances);

		///
		/// Update instance vertex array.
		///
		/// \param instances Set of instance coords to spawn each particle at. Z is opacity.
		///
		void update_instances(std::span<glm::vec3> instances);

		///
		/// Activate sprite context.
		///
		void bind() noexcept override;

		///
		/// Deactivate sprite context.
		///
		void unbind() noexcept override;

	private:
		///
		/// InstanceBuffer object.
		///
		qs::InstanceBuffer m_instance_buffer;
	};

	template<is_buffer BufferType>
	inline void ParticleInstance::create(const float tex_x, const float tex_y)
	{
		auto v1 = qs::make_vertex<qs::SpriteVertex>(0.0f, 0.0f, tex_x, tex_y);
		auto v2 = qs::make_vertex<qs::SpriteVertex>(0.0f + m_width, 0.0f, tex_x + m_width, tex_y);
		auto v3 = qs::make_vertex<qs::SpriteVertex>(0.0f + m_width, 0.0f + m_height, tex_x + m_width, tex_y + m_height);
		auto v4 = qs::make_vertex<qs::SpriteVertex>(0.0f, 0.0f + m_height, tex_x, tex_y + m_height);

		std::array<unsigned int, 6> ib_arr = {0, 1, 3, 1, 2, 3};
		m_vb.create<qs::SpriteVertex, BufferType>({v1, v2, v3, v4});
		m_ib.create<qs::BufferStatic>(ib_arr);

		m_layout.add<qs::SpriteVertex, qs::VAPosition>(2);
		m_layout.add<qs::SpriteVertex, qs::VATexel>(2);

		m_va.create<qs::SpriteVertex>(m_vb, m_ib, m_layout);
	}
} // namespace qs

#endif