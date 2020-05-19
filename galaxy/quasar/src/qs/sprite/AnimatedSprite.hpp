///
/// AnimatedSprite.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_ANIMATEDSPRITE_HPP_
#define QUASAR_ANIMATEDSPRITE_HPP_

#include "qs/core/Animated.hpp"
#include "qs/sprite/Sprite.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// Everything you need to draw an animated sprite.
	///
	class AnimatedSprite final : public qs::Sprite, public qs::Animated
	{
		///
		/// So update() does not need to be exposed publicly.
		///
		friend class Renderer;
	public:
		///
		/// Constructor.
		///
		AnimatedSprite() noexcept;

		///
		/// Destructor.
		///
		virtual ~AnimatedSprite() noexcept = default;

		///
		/// Updates.
		///
		void update() noexcept override;
	};
}

#endif