///
/// Animation.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_ANIMATION_HPP_
#define GALAXY_COMPONENTS_ANIMATION_HPP_

#include <robin_hood.h>

#include <nlohmann/json_fwd.hpp>
#include <qs/sprite/AnimatedBatchSprite.hpp>

namespace galaxy
{
	namespace components
	{
		///
		/// Stores data for an entity about its animations.
		///
		class Animation final
		{
		public:
			///
			/// Default constructor.
			///
			Animation();

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			Animation(const nlohmann::json& json);

			///
			/// Copy constructor.
			///
			Animation(const Animation&) = delete;

			///
			/// Move constructor.
			///
			Animation(Animation&&);

			///
			/// Copy assignment operator.
			///
			Animation& operator=(const Animation&) = delete;

			///
			/// Move assignment operator.
			///
			Animation& operator=(Animation&&);

			///
			/// Destructor.
			///
			~Animation() = default;

			///
			/// Animation object.
			///
			qs::AnimatedBatchSprite m_abs;
		};
	}
} // namespace galaxy

#endif