///
/// Sprite2D.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_SPRITE2D_HPP_
#define QUASAR_SPRITE2D_HPP_

#include "qs/core/Shader.hpp"
#include "qs/vertex/VertexArray.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// Primitive sprite object.
	///
	class Sprite2D final
	{
	public:
		///
		/// Constructor.
		///
		Sprite2D();

		///
		/// Destructor.
		///
		~Sprite2D() noexcept;

		///
		/// \brief Draws sprite to screen.
		/// This is inefficient and you should use Renderer::submit() instead.
		///
		void draw() noexcept;

		///
		/// Resets all internal data, without having to destruct and reconstruct object.
		///
		void reset();

	private:
	};
}

#endif