///
/// Renderer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_RENDERER_HPP_
#define GALAXY_GRAPHICS_RENDERER_HPP_

#include <string>
#include <vector>

#include "galaxy/components/Sprite.hpp"
#include "galaxy/components/Transform.hpp"
#include "galaxy/ecs/Entity.hpp"
#include "galaxy/graphics/Camera.hpp"
#include "galaxy/graphics/SpriteBatch.hpp"

namespace galaxy
{
	namespace components
	{
		class Circle;
		class Ellipse;
		class Line;
		class Point;
		class Polygon;
		class Sprite;
		class Text;
	} // namespace components

	namespace ui
	{
		class GUI;
	} // namespace ui

	namespace graphics
	{
		class ParticleGenerator;
		class Shader;
		class RenderTexture;

		///
		/// OpenGL 2D renderer for drawing VA with transforms, shaders and textures.
		///
		class Renderer final
		{
			friend class ui::GUI;

		public:
			static void init(const unsigned int max_quads, std::string_view batch_shader);

			static void submit_point(components::Point* point, components::Transform* transform, Shader* shader);
			static void submit_line(components::Line* line, components::Transform* transform, Shader* shader);
			static void submit_sprite(components::Sprite* sprite, components::Transform* transform, Shader* shader);
			static void submit_text(components::Text* text, components::Transform* transform, Shader* shader);
			static void submit_batched_sprite(Camera& camera);

			static void submit_lineloop(components::Circle* circle, components::Transform* transform, Shader* shader);
			static void submit_lineloop(components::Polygon* polygon, components::Transform* transform, Shader* shader);
			static void submit_lineloop(components::Ellipse* ellipse, components::Transform* transform, Shader* shader);

			static void draw_batch(graphics::SpriteBatch* sb, Camera& camera);
			static void draw_sprite_to_texture(components::Sprite* sprite, components::Transform* transform, Shader* shader, RenderTexture* target);
			static void draw_particles(graphics::ParticleGenerator* gen, Camera& camera);

			inline static std::vector<Shader*> m_post_shaders;
			inline static std::unique_ptr<SpriteBatch> m_batch;

		private:
			inline static Shader* m_batch_shader;
		};
	} // namespace graphics
} // namespace galaxy

#endif