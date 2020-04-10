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
	class Sprite2D;
	//class BatchSprite2D;
	class Shader;
	class VertexArray;
	class RenderTexture;

	///
	/// OpenGL 2D batch renderer for drawing VA with transforms, shaders and textures.
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
		void drawPoint(qs::VertexArray& va, qs::Shader& shader) noexcept;

		///
		///
		///
		void drawLine(qs::VertexArray& va, qs::Shader& shader) noexcept;

		///
		///
		///
		void drawTriangle(qs::VertexArray& va) noexcept;
		
		///
		///
		///
		void drawQuad(qs::VertexArray& va) noexcept;
		
		///
		///
		///
		void drawCircle(qs::VertexArray& va, qs::Shader& shader) noexcept;

		///
		///
		///
		void drawPolygon(qs::VertexArray& va, qs::Shader& shader) noexcept;

		///
		/// Draw a sprite.
		///
		/// \param sprite Sprite to draw to screen.
		/// \param shader Shader to apply to sprite. You must have called bind() already!
		///
		void drawSprite2D(qs::Sprite2D& sprite, qs::Shader& shader) noexcept;

		///
		/// Draw to render texture.
		///
		/// \param sprite Sprite to draw to screen.
		/// \param shader Shader to apply to sprite. You must have called bind() already!
		///
		void drawSpriteToTexture(qs::Sprite2D& sprite, qs::RenderTexture& rt, qs::Shader& shader) noexcept;

		///
		/// Draw a batched sprite.
		///
		/// \param spritebatch Spritebatch to draw to screen.
		/// \param shader Shader to apply to sprite. You must have called bind() already!
		///
		//void drawBatchSprite(qs::BatchSprite2D* spritebatch, qs::Shader& shader);
	};
}

#endif