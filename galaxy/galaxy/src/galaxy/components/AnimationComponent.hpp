///
/// AnimationComponent.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_ANIMATIONCOMPONENT_HPP_
#define GALAXY_ANIMATIONCOMPONENT_HPP_

#include <unordered_map>

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
		/// Destructor.
		///
		~AnimationComponent() noexcept = default;

		///
		/// Animation object.
		///
		qs::AnimatedBatchSprite m_abs;
	};
} // namespace galaxy

#endif