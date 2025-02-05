///
/// Animated.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_ANIMATED_HPP_
#define GALAXY_COMPONENTS_ANIMATED_HPP_

#include "galaxy/fs/Serializable.hpp"
#include "galaxy/graphics/anim/Animation.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// Allows for an entity to be animated.
		///
		class Animated final : public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			Animated();

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			Animated(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			Animated(Animated&&);

			///
			/// Move assignment operator.
			///
			Animated& operator=(Animated&&);

			///
			/// Destructor.
			///
			virtual ~Animated();

			///
			/// Sets the active animation.
			///
			/// \param animation Animation to use.
			///
			void set_animation(const std::string& animation);

			///
			/// Serializes object.
			///
			/// \return JSON object containing data to be serialized.
			///
			[[nodiscard]]
			nlohmann::json serialize() override;

			///
			/// Deserializes from object.
			///
			/// \param json Json object to retrieve data from.
			///
			void deserialize(const nlohmann::json& json) override;

		private:
			///
			/// Copy constructor.
			///
			Animated(const Animated&) = delete;

			///
			/// Copy assignment operator.
			///
			Animated& operator=(const Animated&) = delete;

		public:
			///
			/// Is the animation paused.
			///
			bool m_paused;

			///
			/// Time spent on current frame.
			///
			double m_duration;

			///
			/// Animation object.
			///
			graphics::Animation* m_anim;
		};
	} // namespace components
} // namespace galaxy

#endif
