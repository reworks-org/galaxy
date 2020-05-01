///
/// Sprite.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_SPRITE_HPP_
#define QUASAR_SPRITE_HPP_

#include "qs/core/Transform.hpp"
#include "qs/renderer/Renderable.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// Everything you need to draw a sprite.
	///
	/// Please note sprite assumes a single texture, with top left at (0, 0).
	/// If your using with a texture atlas you will need to manually
	/// set the texture top left coords using setTexels().
	///
	class Sprite final : public qs::Renderable, public qs::Transform
	{
		///
		/// So update() does not need to be exposed publicly.
		///
		friend class Renderer;

	public:
		///
		/// Constructor.
		///
		Sprite() noexcept;

		///
		/// Destructor.
		///
		~Sprite() noexcept override;

	private:
		///
		/// Update opacity and texture data.
		///
		void update() noexcept;
	};
}

#endif