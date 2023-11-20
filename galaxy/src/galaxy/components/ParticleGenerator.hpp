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
	} // namespace systems

	namespace components
	{
		///
		///
		///
		class ParticleGenerator final : public fs::Serializable, public graphics::Renderable
		{
			friend class systems::RenderSystem;

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
			/// Pointer constructor.
			///
			/// \param ptr Data to COPY into this component.
			///
			ParticleGenerator(ParticleGenerator* ptr);

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
			///
			///
			void generate(const glm::vec2& pos, const std::string& texture, const int layer, const int count);

			///
			///
			///
			void reset();

			///
			///
			///
			void reset(const unsigned int index);

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

		  private:
			///
			///
			///
			void generate_buffers();

		  public:
			///
			///
			///
			int m_count;

			///
			///
			///
			bool m_randomize_position;

			///
			///
			///
			bool m_randomize_initial_vel;

			///
			///
			///
			bool m_randomize_life;

			///
			///
			///
			bool m_randomize_scale;

			///
			///
			///
			bool m_keep_scale_aspect_ratio;

			///
			///
			///
			bool m_randomize_colour;

			///
			///
			///
			bool m_randomize_colour_alpha;

			///
			///
			///
			graphics::ParticleSpread m_spread;

			///
			///
			///
			float m_spread_radius;

			///
			///
			///
			glm::vec2 m_min_rect_spread;

			///
			///
			///
			glm::vec2 m_max_rect_spread;

			///
			///
			///
			glm::vec2 m_fixed_vel;

			///
			///
			///
			glm::vec2 m_min_vel;

			///
			///
			///
			glm::vec2 m_max_vel;

			///
			///
			///
			float m_fixed_life;

			///
			///
			///
			float m_min_life;

			///
			///
			///
			float m_max_life;

			///
			///
			///
			glm::vec2 m_fixed_scale;

			///
			///
			///
			glm::vec2 m_min_scale;

			///
			///
			///
			glm::vec2 m_max_scale;

			///
			///
			///
			glm::vec3 m_fixed_colour;

			///
			///
			///
			glm::vec3 m_min_colour;

			///
			///
			///
			glm::vec3 m_max_colour;

			///
			///
			///
			float m_fixed_alpha;

			///
			///
			///
			float m_min_alpha;

			///
			///
			///
			float m_max_alpha;

			///
			///
			///
			meta::vector<graphics::Particle> m_particles;

		  private:
			///
			///
			///
			unsigned int m_instance;

			///
			///
			///
			unsigned int m_vao;

			///
			///
			///
			unsigned int m_vbo;

			///
			///
			///
			unsigned int m_ebo;

			///
			///
			///
			std::string m_texture;

			///
			/// Starting pos.
			///
			glm::vec2 m_start_pos;

			///
			/// Opengl texture id.
			///
			unsigned int m_tex_id;

			///
			/// Object z-level for drawing.
			///
			int m_layer;
		};
	} // namespace components
} // namespace galaxy

#endif
