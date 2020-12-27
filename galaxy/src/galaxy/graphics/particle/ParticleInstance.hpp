///
/// ParticleInstance.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_PARTICLE_PARTICLEINSTANCE_HPP_
#define GALAXY_GRAPHICS_PARTICLE_PARTICLEINSTANCE_HPP_

#include "galaxy/graphics/texture/Texture.hpp"
#include "galaxy/graphics/vertex/VertexData.hpp"

namespace galaxy
{
	namespace graphics
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
			void bind() override;

			///
			/// Deactivate sprite context.
			///
			void unbind() override;

		private:
			///
			/// InstanceBuffer object.
			///
			InstanceBuffer m_instance_buffer;
		};

		template<is_buffer BufferType>
		inline void ParticleInstance::create(const float tex_x, const float tex_y)
		{
			auto v1 = make_vertex<SpriteVertex>(0.0f, 0.0f, tex_x, tex_y);
			auto v2 = make_vertex<SpriteVertex>(0.0f + m_width, 0.0f, tex_x + m_width, tex_y);
			auto v3 = make_vertex<SpriteVertex>(0.0f + m_width, 0.0f + m_height, tex_x + m_width, tex_y + m_height);
			auto v4 = make_vertex<SpriteVertex>(0.0f, 0.0f + m_height, tex_x, tex_y + m_height);

			std::array<unsigned int, 6> ib_arr = {0, 1, 3, 1, 2, 3};
			std::vector<SpriteVertex> vb_arr   = {v1, v2, v3, v4};

			m_vb.create<SpriteVertex, BufferType>(vb_arr);
			m_ib.create<BufferStatic>(ib_arr);

			m_layout.add<SpriteVertex, VAPosition>(2);
			m_layout.add<SpriteVertex, VATexel>(2);

			m_va.create<SpriteVertex>(m_vb, m_ib, m_layout);
		}
	} // namespace graphics
} // namespace galaxy

#endif