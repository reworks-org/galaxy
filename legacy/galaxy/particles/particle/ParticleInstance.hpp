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
			ParticleInstance() noexcept = default;

			///
			/// Move constructor.
			///
			ParticleInstance(ParticleInstance&&) noexcept;

			///
			/// Move assignment operator.
			///
			ParticleInstance& operator=(ParticleInstance&&) noexcept;

			///
			/// Destructor.
			///
			virtual ~ParticleInstance() noexcept = default;

			///
			/// \brief Creates the internal vertex array.
			///
			/// BufferType Fixed or dynamic buffer.
			///
			/// \param tex_x Optional texture x pos.
			/// \param tex_y Optional texture y pos.
			///
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
			/// Copy constructor.
			///
			ParticleInstance(const ParticleInstance&) = delete;

			///
			/// Copy assignment operator.
			///
			ParticleInstance& operator=(const ParticleInstance&) = delete;

		private:
			///
			/// InstanceBuffer object.
			///
			InstanceBuffer m_instance_buffer;
		};
	} // namespace graphics
} // namespace galaxy

#endif