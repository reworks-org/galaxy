///
/// AnimatedSprite.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_ANIM_ANIMATEDSPRITE_HPP_
#define GALAXY_GRAPHICS_ANIM_ANIMATEDSPRITE_HPP_

#include "galaxy/graphics/anim/Animated.hpp"
#include "galaxy/graphics/sprite/Sprite.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Everything you need to draw an animated sprite.
		///
		class AnimatedSprite final : public Sprite, public Animated
		{
		public:
			///
			/// Constructor.
			///
			AnimatedSprite();

			///
			/// Copy constructor.
			///
			AnimatedSprite(const AnimatedSprite&) = delete;

			///
			/// Copy assignment operator.
			///
			AnimatedSprite& operator=(const AnimatedSprite&) = delete;

			///
			/// Destructor.
			///
			virtual ~AnimatedSprite() = default;

			///
			/// Update GL data.
			///
			/// \param dt DeltaTime.
			///
			void update(const double dt) override;
		};
	} // namespace graphics
} // namespace galaxy

#endif