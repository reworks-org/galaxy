///
/// Renderer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_RENDERER_HPP_
#define GALAXY_GRAPHICS_RENDERER_HPP_

#include <span>
#include <vector>

namespace galaxy
{
	namespace graphics
	{
		///
		/// Forward decs.
		///
		class Line;
		class Point;
		class Shader;
		class Circle;
		class Sprite;
		class AnimatedSprite;
		class SpriteBatch;
		class RenderTexture;
		class Text;
		class ParticleGenerator;
		class PostEffect;

		///
		/// \brief OpenGL 2D renderer for drawing VA with transforms, shaders and textures.
		///
		/// ONLY THE RENDERER IS ALLOWED TO UNBIND/BIND SHADERS.
		///
		class Renderer
		{
		public:
			///
			/// Default constructor.
			///
			Renderer() = default;

			///
			/// Default destructor.
			///
			virtual ~Renderer() = default;

			///
			/// \brief Draw a point.
			///
			/// Must have uniform(s):
			/// u_cameraProj, u_cameraView, u_point_size, u_colour
			///
			/// \param point Point to draw.
			/// \param shader Shader to apply when drawing.
			///
			void draw_point(Point& point, Shader& shader);

			///
			/// \brief Draw a line.
			///
			/// Must have uniform(s):
			/// u_cameraProj, u_cameraView, u_point_size, u_colour
			///
			/// \param line Line to draw.
			///
			void draw_line(Line& line, Shader& shader);

			///
			/// \brief Draw a circle.
			///
			/// Must have uniform(s):
			/// u_cameraProj, u_cameraView
			///
			/// \param circle Circle to draw.
			///
			void draw_circle(Circle& circle, Shader& shader);

			///
			/// \brief Draw a sprite.
			///
			/// Must have uniform(s):
			/// u_transform, u_opacity, u_width, u_height, u_cameraProj, u_cameraView
			///
			/// \param sprite Sprite to draw to screen.
			/// \param shader Shader to apply to sprite. You must have called bind() already!
			///
			void draw_sprite(Sprite& sprite, Shader& shader);

			///
			/// \brief Draw an animated sprite.
			///
			/// Must have uniform(s):
			/// u_transform, u_opacity, u_width, u_height, u_cameraProj, u_cameraView
			///
			/// \param sprite Sprite to draw to screen.
			/// \param shader Shader to apply to sprite. You must have called bind() already!
			///
			void draw_animated_sprite(AnimatedSprite& sprite, Shader& shader);

			///
			/// \brief Draw a spritebatch.
			///
			/// Must have uniform(s):
			/// u_width, u_height, u_cameraProj, u_cameraView
			///
			/// \param spritebatch SpriteBatch to draw to screen.
			/// \param shader Shader to apply to sprite. You must have called bind() already!
			///
			void draw_sprite_batch(SpriteBatch& sprite_batch, Shader& shader);

			///
			/// \brief Draw a sprite to a texture.
			///
			/// Must have uniform(s):
			/// u_transform, u_opacity, u_width, u_height, u_projection
			///
			/// \param sprite Sprite to draw. Pointer so you can cast up other sprite type objects.
			/// \param target Target to draw to.
			/// \param shader Shader to apply to sprite. You must have called bind() already!
			///
			void draw_sprite_to_texture(Sprite* sprite, RenderTexture& target, Shader& shader);

			///
			/// \brief Draw text to screen.
			///
			/// Must have uniform(s):
			/// u_transform, u_opacity, u_cameraView, u_cameraProj
			///
			/// \param text Text to draw.
			/// \param shader Shader to apply to text. You must have called bind() already!
			///
			void draw_text(Text& text, Shader& shader);

			///
			/// \brief Draw particles to screen.
			///
			/// Must have uniform(s):
			/// u_cameraView, u_cameraProj, u_opacity, u_width, u_height
			///
			/// \param particles Particles to draw to screen.
			/// \param shader Shader to apply to particles. You must have called bind() already!
			///
			void draw_particles(ParticleGenerator& particle_gen, Shader& shader);

			///
			/// Add a post effect to the final output of the renderer.
			///
			/// \param effect Effect to add.
			///
			void add_post_effect(PostEffect* effect);

			///
			/// Get post effects to apply.
			///
			/// \return Span of post-effect pointers.
			///
			[[nodiscard]] std::span<PostEffect*> get_post_effects();

			///
			/// Clear all post effects.
			///
			void clear_post_effects();

		private:
			///
			/// List of effects to apply.
			///
			std::vector<PostEffect*> m_effects;
		};
	} // namespace graphics
} // namespace galaxy

#endif