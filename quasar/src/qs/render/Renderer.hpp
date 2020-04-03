///
/// Renderer.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_RENDERER_HPP_
#define QUASAR_RENDERER_HPP_

#include "qs/core/Texture.hpp"
#include "qs/transforms/Transform.hpp"

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
		Renderer();

		///
		/// Destructor.
		///
		~Renderer() noexcept;

		///
		/// Does not take ownership of the sprite - you MUST keep it alive.
		///
		void submit(const const qs::Texture& texture) noexcept;

	private:
		///
		/// The final sprite the batch renderer draws to the screen.
		///
	};
}

#endif