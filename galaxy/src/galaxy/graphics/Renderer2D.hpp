///
/// Renderer2D.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_RENDERER2D_HPP_
#define GALAXY_GRAPHICS_RENDERER2D_HPP_

#include "galaxy/graphics/Camera2D.hpp"
#include "galaxy/graphics/Shader.hpp"
#include "galaxy/graphics/UniformBuffer.hpp"

///
/// Convenience macro.
///
#define RENDERER_2D galaxy::graphics::Renderer2D::inst

namespace galaxy
{
	namespace components
	{
		class Primitive2D;
		class Sprite;
		class Text;
		class Transform2D;
	} // namespace components

	namespace graphics
	{
		class RenderTexture;
		class SpriteBatch;

		///
		/// OpenGL 2D renderer for drawing VA with transforms, shaders and textures.
		///
		class Renderer2D final
		{
		public:
			///
			/// Destructor.
			///
			~Renderer2D() noexcept = default;

			///
			/// Retrieve Renderer instance.
			///
			/// \return Reference to renderer.
			///
			[[nodiscard]] static Renderer2D& inst() noexcept;

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
			/// Draw a point primitive.
			///
			/// \param data Rendering data.
			/// \param transform Transform of primitive.
			///
			void draw_point(components::Primitive2D* data, components::Transform2D* transform);

			///
			/// Draw a line primitive.
			///
			/// \param data Rendering data.
			/// \param transform Transform of primitive.
			///
			void draw_line(components::Primitive2D* data, components::Transform2D* transform);

			///
			/// Draw a connecting line primitive.
			///
			/// \param data Rendering data.
			/// \param transform Transform of primitive.
			///
			void draw_lineloop(components::Primitive2D* data, components::Transform2D* transform);

			///
			/// Draw text.
			///
			/// \param text Text object to draw.
			/// \param transform Transform of text.
			///
			void draw_text(components::Text* text, components::Transform2D* transform);

			///
			/// Draw a sprite.
			///
			/// \param sprite Sprite to draw, seperate to spritebatches.
			/// \param transform Transform of sprite.
			///
			void draw_sprite(components::Sprite* sprite, components::Transform2D* transform);

			///
			/// Draw a sprite to a framebuffer.
			///
			/// \param sprite Sprite to draw.
			/// \param transform Transform of sprite.
			/// \param target Target framebuffer to draw sprite to.
			///
			void draw_sprite_to_target(components::Sprite* sprite, components::Transform2D* transform, RenderTexture* target);

			///
			/// Draw a spritebatch.
			///
			/// \param batch Pointer to SpriteBatch to draw.
			///
			void draw_spritebatch(SpriteBatch* batch);

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
		};
	} // namespace graphics
} // namespace galaxy

#endif