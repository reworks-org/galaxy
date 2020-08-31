///
/// AnimatedBatchSprite.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_ANIMATEDBATCHSPRITE_HPP_
#define QUASAR_ANIMATEDBATCHSPRITE_HPP_

#include "qs/anim/Animated.hpp"
#include "qs/sprite/BatchedSprite.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// Everything you need to draw an animated sprite.
	///
	class AnimatedBatchSprite final : public qs::BatchedSprite, public qs::Animated
	{
	public:
		///
		/// Constructor.
		///
		AnimatedBatchSprite();

		///
		/// Copy constructor.
		///
		AnimatedBatchSprite(const AnimatedBatchSprite&) noexcept = delete;

		///
		/// Move constructor.
		///
		AnimatedBatchSprite(AnimatedBatchSprite&&) noexcept = default;

		///
		/// Copy assignment operator.
		///
		AnimatedBatchSprite& operator=(const AnimatedBatchSprite&) noexcept = delete;

		///
		/// Move assignment operator.
		///
		AnimatedBatchSprite& operator=(AnimatedBatchSprite&&) noexcept = default;

		///
		/// Destructor.
		///
		virtual ~AnimatedBatchSprite() noexcept = default;

		///
		/// Update GL data.
		///
		/// \param dt DeltaTime.
		///
		void update(const double dt) override;
	};
} // namespace qs

#endif