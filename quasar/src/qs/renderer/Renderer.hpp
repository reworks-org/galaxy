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
	class Shader;
	class Circle;
	class Camera;
	class VertexArray;
	class IndexBuffer;
	struct LightSource;
	class RenderTexture;

	///
	/// \brief OpenGL 2D renderer for drawing VA with transforms, shaders and textures.
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
		/// Draw a sprite.
		///
		/// \param sprite Sprite to draw to screen.
		/// \param shader Shader to apply to sprite. You must have called bind() already!
		///
		void drawSprite(qs::Sprite& sprite, qs::Shader& shader) noexcept;

		///
		/// Draw VertexArray to render texture.
		///
		/// \param va Vertex Array to use when drawing.
		/// \param ib IndexBuffer to use when drawing.
		/// \param rt Target to draw to.
		/// \param shader Shader to apply to va. You must have called bind() already!
		///
		void drawVAToTexture(qs::VertexArray& va, qs::IndexBuffer& ib, qs::RenderTexture& rt, qs::Shader& shader) noexcept;

		///
		/// Draw to render texture.
		///
		/// \param sprite Sprite to draw to screen.
		/// \param rt Target to draw to.
		/// \param shader Shader to apply to sprite. You must have called bind() already!
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
		///
		///
		void drawScene(qs::Sprite& sprite, qs::Camera& camera, qs::LightSource& ls) noexcept;
	};
}

#endif