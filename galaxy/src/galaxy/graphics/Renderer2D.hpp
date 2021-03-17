///
/// Renderer2D.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_RENDERER2D_HPP_
#define GALAXY_GRAPHICS_RENDERER2D_HPP_

#include "galaxy/graphics/Camera.hpp"
#include "galaxy/graphics/SpriteBatch.hpp"

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
			static void init(const unsigned int max_quads, std::string_view batch_shader);
			static void clean_up();

			static void draw_point(components::Primitive2D* data, components::Transform* transform, Shader* shader);
			static void draw_line(components::Primitive2D* data, components::Transform* transform, Shader* shader);
			static void draw_lineloop(components::Primitive2D* data, components::Transform* transform, Shader* shader);
			static void draw_spritebatch(Camera& camera);
			static void draw_text(components::Text* text, components::Transform* transform, Shader* shader);
			static void draw_sprite(components::Sprite* sprite, components::Transform* transform, Shader* shader);

			static void draw_batch(graphics::SpriteBatch* sb, Camera& camera);
			static void draw_texture_to_target(graphics::VertexData* vertex_data, graphics::BaseTexture* texture, const glm::mat4& transform, Shader* shader, RenderTexture* target);
			static void draw_sprite_to_target(components::Sprite* sprite, components::Transform* transform, Shader* shader, RenderTexture* target);

		public:
			inline static std::vector<Shader*> m_post_shaders;
			inline static std::unique_ptr<SpriteBatch> m_batch;

		private:
			inline static Shader* m_batch_shader;
		};
	} // namespace graphics
} // namespace galaxy

#endif