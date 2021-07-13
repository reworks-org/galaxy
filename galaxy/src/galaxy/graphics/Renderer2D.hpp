///
/// Renderer2D.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_RENDERER2D_HPP_
#define GALAXY_GRAPHICS_RENDERER2D_HPP_

#include "galaxy/graphics/Camera2D.hpp"
#include "galaxy/graphics/UniformBuffer.hpp"
#include "galaxy/graphics/RenderLayer.hpp"

///
/// Convenience macro.
///
#define RENDERER_2D galaxy::graphics::Renderer2D::inst

namespace galaxy
{
	namespace components
	{
		class BatchSprite;
		class ParticleEffect;
		class Primitive2D;
		class Sprite;
		class Text;
		class Transform2D;
	} // namespace components

	namespace graphics
	{
		class RenderTexture;

		///
		/// OpenGL 2D renderer for drawing VA with transforms, shaders and textures.
		///
		class Renderer2D final
		{
			using RenderLayers = robin_hood::unordered_flat_map<std::string, RenderLayer>;

		public:
			///
			/// Destructor.
			///
			~Renderer2D() noexcept;

			///
			/// Retrieve Renderer instance.
			///
			/// \return Reference to renderer.
			///
			[[nodiscard]] static Renderer2D& inst() noexcept;

			///
			/// Initialize render layers.
			///
			/// \param layers Path to renderlayers.json.
			///
			void init(std::string_view layers);

			///
			/// Clean up data.
			///
			void clear() noexcept;

			///
			/// Update Camera UBO.
			///
			/// \param camera Data to be buffered to UBO.
			///
			void buffer_camera(Camera2D& camera);

			///
			/// Bind point shader.
			///
			void bind_point() noexcept;

			///
			/// Bind line shader.
			///
			void bind_line() noexcept;

			///
			/// Bind text shader.
			///
			void bind_text() noexcept;

			///
			/// Bind sprite shader.
			///
			void bind_sprite() noexcept;

			///
			/// Bind Render To Texture shader.
			///
			void bind_rtt() noexcept;

			///
			/// Bind SpriteBatch shader.
			///
			void bind_sb_shader() noexcept;

			///
			/// Bind instance shader.
			///
			void bind_instance_shader() noexcept;

			///
			/// Submit a primitive to the renderer.
			///
			/// \param data Rendering data.
			/// \param transform Transform of primitive.
			///
			void submit(components::Primitive2D* data, components::Transform2D* transform);

			///
			/// Submit text to the renderer.
			///
			/// \param text Text object to draw.
			/// \param transform Transform of text.
			///
			void submit(components::Text* text, components::Transform2D* transform);

			///
			/// Submit a sprite to the renderer.
			///
			/// \param sprite Sprite to draw, seperate to spritebatches.
			/// \param transform Transform of sprite.
			///
			void submit(components::Sprite* sprite, components::Transform2D* transform);

			///
			/// Submit a batched sprite to the renderer.
			///
			/// \param batch Batched sprite data.
			/// \param transform Transform of batch sprite.
			///
			void submit(components::BatchSprite* batch, components::Transform2D* transform);

			///
			/// Submit a particle effect to the renderer.
			///
			/// \param particle_effect Particle effect data.
			///
			void submit(components::ParticleEffect* particle_effect);

			///
			/// Prepare renderer for drawing.
			///
			void prepare();

			///
			/// Draw everything.
			///
			void draw();

			///
			/// Draw a sprite to a framebuffer.
			///
			/// \param sprite Sprite to draw.
			/// \param transform Transform of sprite.
			/// \param target Target framebuffer to draw sprite to.
			///
			void draw_sprite_to_target(components::Sprite* sprite, components::Transform2D* transform, RenderTexture* target);

		private:
			///
			/// Constructor.
			///
			Renderer2D() noexcept;

			///
			/// Camera uniform buffer data.
			///
			UniformBuffer m_camera_ubo;

			///
			/// Point shader.
			///
			Shader m_point_shader;

			///
			/// Line shader.
			///
			Shader m_line_shader;

			///
			/// Text shader.
			///
			Shader m_text_shader;

			///
			/// Sprite shader.
			///
			Shader m_sprite_shader;

			///
			/// Render To Texture shader.
			///
			Shader m_rtt_shader;

			///
			/// SpriteBatch shader.
			///
			Shader m_spritebatch_shader;

			///
			/// Instance shader.
			///
			Shader m_instance_shader;

			///
			/// Render layer data.
			///
			RenderLayers m_layer_data;

			///
			/// Sorted renderlayer pointers.
			///
			std::vector<RenderLayer*> m_layers;
		};
	} // namespace graphics
} // namespace galaxy

#endif