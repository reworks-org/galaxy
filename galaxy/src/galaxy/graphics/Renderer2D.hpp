///
/// Renderer2D.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_RENDERER2D_HPP_
#define GALAXY_GRAPHICS_RENDERER2D_HPP_

#include <robin_hood.h>

#include "galaxy/graphics/camera/Camera2D.hpp"
#include "galaxy/graphics/SpriteBatch.hpp"

///
/// Convenience macro.
///
#define RENDERER_2D galaxy::graphics::Renderer2D::inst

namespace galaxy
{
	namespace components
	{
		class BatchSprite;
		class Primitive2D;
		class Sprite;
		class Text;
	} // namespace components

	namespace graphics
	{
		class Shader;
		class RenderTexture;

		///
		/// OpenGL 2D renderer for drawing VA with transforms, shaders and textures.
		///
		class Renderer2D final
		{
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
			/// Initialize renderer2d.
			///
			/// \param max_quads Maximum quads per spritebatch.
			/// \param batch_shader ID of the shader to use for spritebatching.
			/// \param framebuffer_sahder ID of the shader to use when drawing renderer contents to the main window framebuffer.
			///
			void init(const unsigned int max_quads, std::string_view batch_shader, std::string_view framebuffer_shader);

			///
			/// Clears all data from renderer, including static data.
			///
			void clean_up();

			///
			/// Creates default spritebatches from a base texture.
			///
			/// \param texture Usually the textureatlas texture.
			///
			void create_default_batches(BaseTexture* texture) noexcept;

			///
			/// Add a batched sprite to be rendered with a spritebatch.
			///
			/// \param batchsprite Pointer to batchsprite from an entity.
			/// \param transform Pointer to transform from an entity.
			/// \param zlevel Level to render sprite at (depth).
			///
			void add_batched_sprite(components::BatchSprite* batchsprite, components::Transform2D* transform, int zlevel);

			///
			/// Calculate spritebatch verticies.
			///
			void calculate_batches();

			///
			/// Clears sprite and batch data.
			///
			void clear();

			///
			/// Prepare framebuffer to recieve render commands.
			///
			void prepare() noexcept;

			///
			/// Bind to take render commands.
			///
			void bind() noexcept;

			///
			/// Unbind and finish rendering.
			///
			void unbind() noexcept;

			///
			/// Render contents to active framebuffer.
			///
			void render();

			///
			/// Resize framebuffer.
			///
			/// \param width New framebuffer width. Usually screen width.
			/// \param height New framebuffer height. Usually screen height.
			///
			void resize(const int width, const int height);

			///
			/// Draw a point primitive.
			///
			/// \param data Rendering data.
			/// \param transform Transform of primitive.
			/// \param shader Shader to draw primitive with.
			///
			void draw_point(components::Primitive2D* data, components::Transform2D* transform, Shader* shader);

			///
			/// Draw a line primitive.
			///
			/// \param data Rendering data.
			/// \param transform Transform of primitive.
			/// \param shader Shader to draw primitive with.
			///
			void draw_line(components::Primitive2D* data, components::Transform2D* transform, Shader* shader);

			///
			/// Draw a connecting line primitive.
			///
			/// \param data Rendering data.
			/// \param transform Transform of primitive.
			/// \param shader Shader to draw primitive with.
			///
			void draw_lineloop(components::Primitive2D* data, components::Transform2D* transform, Shader* shader);

			///
			/// Draw all spritebatches.
			///
			/// \param camera Camera projection to draw with.
			///
			void draw_spritebatches(Camera2D& camera);

			///
			/// Draw text.
			///
			/// \param text Text object to draw.
			/// \param transform Transform of text.
			/// \param shader Shader to draw text with.
			///
			void draw_text(components::Text* text, components::Transform2D* transform, Shader* shader);

			///
			/// Draw a sprite.
			///
			/// \param sprite Sprite to draw, seperate to spritebatches.
			/// \param transform Transform of sprite.
			/// \param shader Shader to draw sprite with.
			///
			void draw_sprite(components::Sprite* sprite, components::Transform2D* transform, Shader* shader);

			///
			/// Draw a textured VAO to a framebuffer.
			///
			/// \param vertex_data Vertex Array Object to draw.
			/// \param texture Texture to map to verticies.
			/// \param transform Transform matrix of VAO.
			/// \param shader Shader to draw VAO with.
			/// \param target Target framebuffer to draw VAO to.
			///
			void draw_texture_to_target(graphics::VertexData* vertex_data, graphics::BaseTexture* texture, const glm::mat4& transform, Shader* shader, RenderTexture* target);

			///
			/// Draw a sprite to a framebuffer.
			///
			/// \param sprite Sprite to draw.
			/// \param transform Transform of sprite.
			/// \param shader Shader to draw sprite with.
			/// \param target Target framebuffer to draw sprite to.
			///
			void draw_sprite_to_target(components::Sprite* sprite, components::Transform2D* transform, Shader* shader, RenderTexture* target);

		private:
			///
			/// Constructor.
			///
			Renderer2D() noexcept;

		public:
			///
			/// Stores spritebatches by zlevel.
			///
			robin_hood::unordered_flat_map<int, std::unique_ptr<SpriteBatch>> m_batches;

		private:
			///
			/// Pointer to texture to draw spritebatches with.
			///
			BaseTexture* m_texture;

			///
			/// Pointer to batch shader.
			///
			Shader* m_batch_shader;

			///
			/// Pointer to framebuffer shader.
			///
			Shader* m_fb_shader;

			///
			/// Total number of quads per spritebatch allowed.
			///
			unsigned int m_max_quads;

			///
			/// Internal framebuffer.
			///
			std::unique_ptr<graphics::RenderTexture> m_framebuffer;

			///
			/// Internal framebuffer VAO.
			///
			std::unique_ptr<components::Sprite> m_fb_sprite;

			///
			/// Internal framebuffer transform.
			///
			components::Transform2D m_fb_transform;
		};
	} // namespace graphics
} // namespace galaxy

#endif