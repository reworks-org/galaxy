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
	// Forward decs.
	class Text;
	class Sprite;
	class Shader;
	class VertexArray;
	class IndexBuffer;
	class RenderTexture;

	///
	/// OpenGL 2D renderer for drawing VA with transforms, shaders and textures.
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
		///
		///
		void drawPoint(qs::Shader& shader) noexcept;

		///
		///
		///
		void drawLine(qs::Shader& shader) noexcept;

		///
		///
		///
		void drawTriangle(qs::Shader& shader) noexcept;
		
		///
		///
		///
		void drawQuad(qs::Shader& shader) noexcept;
		
		///
		///
		///
		void drawCircle(qs::Shader& shader) noexcept;

		///
		///
		///
		void drawPolygon(qs::Shader& shader) noexcept;

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
	};
}

#endif