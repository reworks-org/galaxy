///
/// Renderer.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_RENDERER_HPP_
#define QUASAR_RENDERER_HPP_

///
/// Core namespace.
///
namespace qs
{
	///
	/// Forward decs.
	///
	class Line;
	class Text;
	class Point;
	class Sprite;
	class SpriteBatch;
	class Character;
	class Shader;
	class Circle;
	class Camera;
	class VertexArray;
	class IndexBuffer;
	struct LightSource;
	class RenderTexture;
	class Renderable;
	class Transform;

	///
	/// \brief OpenGL 2D renderer for drawing VA with transforms, shaders and textures.
	///
	/// ONLY THE RENDERER IS ALLOWED TO UNBIND/BIND SHADERS.
	///
	class Renderer final
	{
	public:
		///
		/// Default constructor.
		///
		Renderer() noexcept = default;

		///
		/// Default destructor.
		///
		~Renderer() noexcept = default;

		///
		/// Draw a point.
		///
		/// \param point Point to draw.
		/// \param shader Shader to apply when drawing.
		///
		void drawPoint(qs::Point& point, qs::Shader& shader) noexcept;

		///
		/// Draw a line.
		///
		/// \param line Line to draw.
		///
		void drawLine(qs::Line& line) noexcept;
		
		///
		/// Draw a circle.
		///
		/// \param circle Circle to draw.
		///
		void drawCircle(qs::Circle& circle) noexcept;

		///
		/// Draw VertexArray to render texture.
		///
		/// \param character (glyph) to draw.
		/// \param rt Target to draw to.
		/// \param shader Shader to apply to va.  CALLS bind() FOR YOU!
		///
		void drawCharacter(qs::Character& character, qs::RenderTexture& rt, qs::Shader& shader) noexcept;

		///
		/// Draw a renderable object.
		///
		/// \param rb Renderable object to draw.
		/// \param transform Transform to apply to renderable.
		/// \param shader Shader to apply.
		///
		void drawRenderable(qs::Renderable* rb, qs::Transform& transform, qs::Shader& shader) noexcept;

		///
		/// Draw a sprite.
		///
		/// \param sprite Sprite to draw to screen.
		/// \param shader Shader to apply to sprite. You must have called bind() already!
		///
		void drawSprite(qs::Sprite& sprite, qs::Shader& shader) noexcept;

		///
		/// Draw a spritebatch.
		///
		/// \param spritebatch SpriteBatch to draw to screen.
		/// \param shader Shader to apply to sprite. You must have called bind() already!
		///
		void drawSpriteBatch(qs::SpriteBatch& spritebatch, qs::Shader& shader) noexcept;

		///
		/// Draw to render texture.
		///
		/// \param sprite Sprite to draw to screen.
		/// \param rt Target to draw to.
		/// \param shader Shader to apply to sprite. CALLS bind() FOR YOU!
		///
		void drawSpriteToTexture(qs::Sprite& sprite, qs::RenderTexture& rt, qs::Shader& shader) noexcept;

		///
		/// Draw text to screen.
		///
		/// \param text Text to draw.
		/// \param shader Shader to apply to text. You must have called bind() already!
		///
		void drawText(qs::Text& text, qs::Shader& shader) noexcept;

		///
		/// \brief Draw a scene.
		///
		/// A scene is where you render all the sprites to a texture then apply a post process shader.
		/// Usually is a lightsource shader.
		/// Shader is bound for you.
		/// LightSource provides shader.
		///
		/// \param spritebatch Framebuffer sprite.
		/// \param camera used to render scene.
		/// \param ls Light source(s) for lighting. Provides its own shader.
		///
		void drawScene(qs::SpriteBatch& spritebatch, qs::Camera& camera, qs::LightSource& ls) noexcept;
	};
}

#endif