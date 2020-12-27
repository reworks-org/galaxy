///
/// AnimatedBatchSprite.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_ANIM_ANIMATEDBATCHSPRITE_HPP_
#define GALAXY_GRAPHICS_ANIM_ANIMATEDBATCHSPRITE_HPP_

#include <nlohmann/json_fwd.hpp>

#include "galaxy/graphics/anim/Animated.hpp"
#include "galaxy/graphics/sprite/BatchedSprite.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Everything you need to draw an animated sprite.
		///
		class AnimatedBatchSprite final : public BatchedSprite, public Animated
		{
		public:
			///
			/// Constructor.
			///
			AnimatedBatchSprite();

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			AnimatedBatchSprite(const nlohmann::json& json);

			///
			/// Copy constructor.
			///
			AnimatedBatchSprite(const AnimatedBatchSprite&) = delete;

			///
			/// Copy assignment operator.
			///
			AnimatedBatchSprite& operator=(const AnimatedBatchSprite&) = delete;

			///
			/// Destructor.
			///
			virtual ~AnimatedBatchSprite() = default;

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