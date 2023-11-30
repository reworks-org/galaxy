///
/// ParticleGenerator.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_PARTICLEGENERATOR_HPP_
#define GALAXY_COMPONENTS_PARTICLEGENERATOR_HPP_

#include <glm/vec3.hpp>

#include "galaxy/fs/Serializable.hpp"
#include "galaxy/graphics/Particle.hpp"
#include "galaxy/graphics/ParticleSpread.hpp"
#include "galaxy/graphics/Renderable.hpp"

namespace galaxy
{
	namespace systems
	{
		class RenderSystem;
		class ParticleSystem;
	} // namespace systems

	namespace components
	{
		///
		/// Allows an entity to emit particle effects.
		///
		class ParticleGenerator final : public fs::Serializable, public graphics::Renderable
		{
			friend class systems::RenderSystem;
			friend class systems::ParticleSystem;

		  public:
			///
			/// Constructor.
			///
			ParticleGenerator();

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			ParticleGenerator(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			ParticleGenerator(ParticleGenerator&&);

			///
			/// Move assignment operator.
			///
			ParticleGenerator& operator=(ParticleGenerator&&);

			///
			/// Destructor.
			///
			virtual ~ParticleGenerator();

			///
			/// Configures and generates the particles to be emitted.
			///
			/// \param pos Starting position of particles.
			/// \param texture Texture for particles to have.
			/// \param layer Rendering layer.
			/// \param count Amount of particles to emit.
			///
			void generate(const glm::vec2& pos, const std::string& texture, const int layer, const int count);

			///
			/// Re-generates particles from existing config.
			///
			void regenerate();

			///
			/// Reset all particle lifetime and positions.
			///
			void reset();

			///
			/// Copy updated particle data to opengl.
			///
			void buffer_instances();

			///
			/// Number of instances to render.
			///
			/// \return Count for number of instances.
			///
			[[nodiscard]] int get_instances() const override;

			///
			/// Get OpenGL rendering mode.
			///
			/// \return Unsigned int.
			///
			[[nodiscard]] unsigned int get_mode() const override;

			///
			/// Get vertex array object.
			///
			/// \return VAO handle.
			///
			[[nodiscard]] unsigned int get_vao() const override;

			///
			/// Gets OpenGL texture id.
			///
			/// \return unsigned int. 0 if no texture.
			///
			[[nodiscard]] unsigned int get_texture() const override;

			///
			/// Get index (element) buffer count.
			///
			/// \return Unsigned int.
			///
			[[nodiscard]] unsigned int get_count() const override;

			///
			/// Get rendering layer.
			///
			/// \return Integer. 0 is valid as a layer. So are negatives.
			///
			[[nodiscard]] int get_layer() const override;

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
			/// Copy assignment operator.
			///
			ParticleGenerator& operator=(const ParticleGenerator&) = delete;

			///
			/// Copy constructor.
			///
			ParticleGenerator(const ParticleGenerator&) = delete;

			///
			/// Reset a specific particle.
			///
			/// \param index Index in the particle list.
			///
			void reset(const unsigned int index);

		  private:
			///
			/// Create initial opengl buffers.
			///
			void generate_buffers();

		  public:
			///
			/// Number of particles.
			///
			int m_count;

			///
			/// Should spawning position be randomized.
			///
			bool m_randomize_position;

			///
			/// Should initial velocity be randomized.
			///
			bool m_randomize_initial_vel;

			///
			/// Should particle life be randomized.
			///
			bool m_randomize_life;

			///
			/// Should particle size be randomized.
			///
			bool m_randomize_scale;

			///
			/// Should particles maintain aspect ratio when scaling.
			///
			bool m_keep_scale_aspect_ratio;

			///
			/// Should particle tint be randomized.
			///
			bool m_randomize_colour;

			///
			/// Should particle opacity be randomized.
			///
			bool m_randomize_colour_alpha;

			///
			/// Defines area in which particles are spawned.
			///
			graphics::ParticleSpread m_spread;

			///
			/// Radius particles should spawn around the emitter.
			///
			float m_spread_radius;

			///
			/// Min spread for a rectangle area.
			///
			glm::vec2 m_min_rect_spread;

			///
			/// Max spread for a rectangle area.
			///
			glm::vec2 m_max_rect_spread;

			///
			/// Default velocity.
			///
			glm::vec2 m_fixed_vel;

			///
			/// Min allowed velocity.
			///
			glm::vec2 m_min_vel;

			///
			/// Max allowed velocity.
			///
			glm::vec2 m_max_vel;

			///
			/// Default life.
			///
			float m_fixed_life;

			///
			/// Min particle life.
			///
			float m_min_life;

			///
			/// Max particle life.
			///
			float m_max_life;

			///
			/// Default scale.
			///
			glm::vec2 m_fixed_scale;

			///
			/// Min scale.
			///
			glm::vec2 m_min_scale;

			///
			/// Max scale.
			///
			glm::vec2 m_max_scale;

			///
			/// Default colour.
			///
			glm::vec3 m_fixed_colour;

			///
			/// Min colour.
			///
			glm::vec3 m_min_colour;

			///
			/// Max colour.
			///
			glm::vec3 m_max_colour;

			///
			/// Default opacity.
			///
			float m_fixed_alpha;

			///
			/// Min allowed opacity.
			///
			float m_min_alpha;

			///
			/// Max allowed opacity.
			///
			float m_max_alpha;

			///
			/// Texture name.
			///
			std::string m_texture;

			///
			/// Starting pos.
			///
			glm::vec2 m_start_pos;
			///
			/// Object z-level for drawing.
			///
			int m_layer;

		  private:
			///
			/// OpenGL instance buffer handle.
			///
			unsigned int m_instance;

			///
			/// OpenGL vertex array object handle.
			///
			unsigned int m_vao;

			///
			/// OpenGL vertex buffer object handle.
			///
			unsigned int m_vbo;

			///
			/// OpenGL element buffer handle.
			///
			unsigned int m_ebo;

			///
			/// Opengl texture id.
			///
			unsigned int m_tex_id;

			///
			/// List of created particles.
			///
			meta::vector<graphics::Particle> m_particles;
		};
	} // namespace components
} // namespace galaxy

#endif
