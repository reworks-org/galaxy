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

			[[nodiscard]] static Renderer2D& inst() noexcept;

			void init(const unsigned int max_quads, std::string_view batch_shader);

			///
			/// Clears all data from renderer, including static data.
			///
			void clean_up();

			void create_default_batches(BaseTexture* texture) noexcept;
			void add_batched_sprite(components::BatchSprite* batchsprite, components::Transform* transform, int zlevel);
			void calculate_batches();

			///
			/// Clears sprite and batch data.
			///
			void clear();

			void draw_point(components::Primitive2D* data, components::Transform* transform, Shader* shader);
			void draw_line(components::Primitive2D* data, components::Transform* transform, Shader* shader);
			void draw_lineloop(components::Primitive2D* data, components::Transform* transform, Shader* shader);
			void draw_spritebatches(Camera& camera);
			void draw_text(components::Text* text, components::Transform* transform, Shader* shader);
			void draw_sprite(components::Sprite* sprite, components::Transform* transform, Shader* shader);

			void draw_texture_to_target(graphics::VertexData* vertex_data, graphics::BaseTexture* texture, const glm::mat4& transform, Shader* shader, RenderTexture* target);
			void draw_sprite_to_target(components::Sprite* sprite, components::Transform* transform, Shader* shader, RenderTexture* target);

		private:
			///
			/// Constructor.
			///
			Renderer2D() noexcept;

		public:
			std::vector<Shader*> m_post_shaders;
			robin_hood::unordered_flat_map<int, std::unique_ptr<SpriteBatch>> m_batches;

		private:
			BaseTexture* m_texture;
			Shader* m_batch_shader;
			unsigned int m_max_quads;
		};
	} // namespace graphics
} // namespace galaxy

#endif