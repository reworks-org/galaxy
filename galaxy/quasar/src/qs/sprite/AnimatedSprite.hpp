///
/// AnimatedSprite.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_ANIMATEDSPRITE_HPP_
#define QUASAR_ANIMATEDSPRITE_HPP_

#include "qs/anim/Animated.hpp"
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
	public:
		///
		/// Constructor.
		///
		AnimatedSprite();

		///
		/// Destructor.
		///
		~AnimatedSprite() noexcept = default;

		///
		/// Update GL data.
		///
		/// \param dt DeltaTime.
		///
		void update(const double dt) override;
	};
} // namespace qs

#endif