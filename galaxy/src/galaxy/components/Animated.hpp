///
/// Animated.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_ANIMATED_HPP_
#define GALAXY_COMPONENTS_ANIMATED_HPP_

#include <robin_hood.h>

#include "galaxy/graphics/animation/Animation.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// Allows for an entity to be animated.
		///
		class Animated final : public fs::Serializable
		{
			using AnimationCache = robin_hood::unordered_node_map<std::string, graphics::Animation>;

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
			/// Add an animation to the object.
			///
			/// \param name ID of animation.
			/// \param looping Does animation repeat.
			/// \param speed Multiplier. Speed of animation.
			/// \param frames Frames in the animation.
			///
			void add(const std::string& name, const bool looping, const double speed, std::span<graphics::Frame> frames);

			///
			/// Change the current animation.
			///
			/// \param animation The name of the animation type to change to as defined in lua file. E.g. "walking" -> "running"
			///
			void set(const std::string& animation);

			///
			/// Play the animation.
			///
			void play();

			///
			/// Play a specific animation.
			///
			/// \param animation Animation to change to to play.
			///
			void play(const std::string& animation);

			///
			/// Pause animation.
			///
			void pause();

			///
			/// \brief Stop the animation.
			///
			/// If you call play() the animation starts from the beginning.
			///
			void stop();

			///
			/// Is the animation paused?
			///
			/// \return Bool.
			///
			[[nodiscard]] bool is_paused() const;

			///
			/// Currently active animation.
			///
			/// \return Pointer to animation.
			///
			[[nodiscard]] graphics::Animation* active();

			///
			/// Serializes object.
			///
			/// \return JSON object containing data to be serialized.
			///
			[[nodiscard]] nlohmann::json serialize() override;

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
			/// Animation cache.
			///
			AnimationCache m_animations;

			///
			/// Current amount of time spent on the frame.
			///
			double m_time_spent_on_frame;

		private:
			///
			/// Currently active animation.
			///
			graphics::Animation* m_active;

			///
			/// Is animation is paused or not.
			///
			bool m_paused;
		};
	} // namespace components
} // namespace galaxy

#endif