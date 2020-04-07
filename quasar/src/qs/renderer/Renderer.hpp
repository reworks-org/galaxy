///
/// Renderer.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_RENDERER_HPP_
#define QUASAR_RENDERER_HPP_

#include "qs/core/Shader.hpp"
#include "qs/graphics/Sprite2D.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// OpenGL 2D batch renderer for drawing VA with transforms, shaders and textures.
	///
	class Renderer final
	{
	public:
		///
		/// Constructor.
		///
		Renderer() noexcept;

		///
		/// Destructor.
		///
		~Renderer() noexcept;

		///
		///
		///
		void drawLine() noexcept;

		///
		///
		///
		void drawTriangle() noexcept;
		
		///
		///
		///
		void drawQuad() noexcept;
		
		///
		///
		///
		void drawCircle() noexcept;

		///
		///
		///
		void drawPolygon() noexcept;

		///
		/// Draw a sprite.
		///
		/// \param sprite Sprite to draw to screen.
		/// \param shader Shader to apply to sprite. You must have called bind() already!
		///
		void drawSprite2D(qs::Sprite2D& sprite, qs::Shader& shader) noexcept;

	private:
	};
}

#endif