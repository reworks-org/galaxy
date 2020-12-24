///
/// AnimationComponent.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_ANIMATIONCOMPONENT_HPP_
#define GALAXY_ANIMATIONCOMPONENT_HPP_

#include <robin_hood.h>

#include <nlohmann/json_fwd.hpp>
#include <qs/sprite/AnimatedBatchSprite.hpp>

namespace galaxy
{
	///
	/// Stores data for an entity about its animations.
	///
	class AnimationComponent final
	{
	public:
		///
		/// Default constructor.
		///
		AnimationComponent();

		///
		/// JSON constructor.
		///
		/// \param json JSON defining object.
		///
		AnimationComponent(const nlohmann::json& json);

		///
		/// Copy constructor.
		///
		AnimationComponent(const AnimationComponent&) = delete;

		///
		/// Move constructor.
		///
		AnimationComponent(AnimationComponent&&);

		///
		/// Copy assignment operator.
		///
		AnimationComponent& operator=(const AnimationComponent&) = delete;

		///
		/// Move assignment operator.
		///
		AnimationComponent& operator=(AnimationComponent&&);

		///
		/// Destructor.
		///
		~AnimationComponent() = default;

		///
		/// Animation object.
		///
		qs::AnimatedBatchSprite m_abs;
	};
} // namespace galaxy

#endif