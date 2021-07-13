///
/// ParticleEffect.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_PARTICLEEFFECT_HPP_
#define GALAXY_COMPONENTS_PARTICLEEFFECT_HPP_

#include "galaxy/fs/Serializable.hpp"
#include "galaxy/graphics/Particle.hpp"
#include "galaxy/graphics/Texture.hpp"
#include "galaxy/graphics/VertexArray.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// Batch Sprite thats rendered with instances.
		///
		class ParticleEffect final : public graphics::Texture, public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			ParticleEffect() noexcept;

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			ParticleEffect(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			ParticleEffect(ParticleEffect&&) noexcept;

			///
			/// Move assignment operator.
			///
			ParticleEffect& operator=(ParticleEffect&&) noexcept;

			///
			/// Destructor.
			///
			virtual ~ParticleEffect() noexcept;

			///
			/// \brief Create particle effect data.
			///
			/// Each parameter will be randomized slightly for each particle instance, including total count.
			///
			/// \param layer Rendering layer.
			/// \param count Total number of particles to have.
			/// \param radius Initial radius to confine particles to.
			/// \param starting_pos Starting position of particles, before offset.
			/// \param velocity Velocity of particles.
			///
			void create(std::string_view layer, const int count, const float radius, const glm::vec2& starting_pos, const glm::vec2& velocity);

			///
			/// Regenerate particles.
			///
			/// \param new_pos Optional. Update the starting position, before offset.
			///
			void regen(std::optional<glm::vec2> new_pos = std::nullopt);

			///
			/// Buffer data into OpenGL.
			///
			void buffer();

			///
			/// Bind sprite.
			///
			void bind() noexcept override;

			///
			/// Unbind sprite.
			///
			void unbind() noexcept override;

			///
			/// Set opacity.
			///
			/// \param opacity 0 - 255.
			///
			void set_opacity(const std::uint8_t opacity) noexcept;

			///
			/// Get rendering layer.
			///
			/// \return Const std::string reference.
			///
			[[nodiscard]] const std::string& get_layer() const noexcept;

			///
			/// Get opacity.
			///
			/// \return Const std::uint8_t.
			///
			[[nodiscard]] const std::uint8_t get_opacity() const noexcept;

			///
			/// Get particle instances.
			///
			/// \return Vector of particles.
			///
			[[nodiscard]] std::vector<graphics::Particle>& get_particles() noexcept;

			///
			/// Gets the VAO.
			///
			/// \return Const uint.
			///
			[[nodiscard]] const unsigned int vao() const noexcept;

			///
			/// Gets the index count.
			///
			/// \return Const int.
			///
			[[nodiscard]] const int index_count() const noexcept;

			///
			/// Get the instance count.
			///
			/// \return Const uint.
			///
			[[nodiscard]] const unsigned int instance_count() const noexcept;

			///
			/// Serializes object.
			///
			/// \return JSON object containing data to be serialized.
			///
			[[nodiscard]] nlohmann::json serialize() override;

			///
			/// Deserializes from object.
			///
			/// \param json Json object to retrieve data from.
			///
			void deserialize(const nlohmann::json& json) override;

		private:
			///
			/// Copy constructor.
			///
			ParticleEffect(const ParticleEffect&) = delete;

			///
			/// Copy assignment operator.
			///
			ParticleEffect& operator=(const ParticleEffect&) = delete;

		private:
			///
			/// Vertex Array Object.
			///
			graphics::VertexArray m_vao;

			///
			/// Instance object.
			///
			graphics::InstanceBuffer m_ibo;

			///
			/// Opacity.
			///
			std::uint8_t m_opacity;

			///
			/// Rendering Layer.
			///
			std::string m_layer;

			///
			/// Instances of each particle.
			///
			std::vector<graphics::Particle> m_instances;

			///
			/// Offsets stored when buffering.
			///
			std::vector<glm::vec2> m_offsets;

			///
			/// Original count.
			///
			int m_count;

			///
			/// Current radius.
			///
			float m_radius;

			///
			/// Current starting position.
			///
			glm::vec2 m_starting_pos;

			///
			/// Current velocity.
			///
			glm::vec2 m_velocity;
		};
	} // namespace components
} // namespace galaxy

#endif