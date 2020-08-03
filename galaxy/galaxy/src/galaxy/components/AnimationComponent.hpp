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
#include <qs/anim/Animation.hpp>

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
		~AnimationComponent();

		///
		/// Change the current animation.
		///
		/// \param animation The name of the animation type to change to as defined in lua file. E.g. "walking" -> "running"
		///
		void set_animation(std::string_view animation);

		///
		/// Play the animation.
		///
		void play();

		///
		/// Play a specific animation.
		///
		/// \param animation Animation to change to to play.
		///
		void play(std::string_view animation);

		///
		/// Pause animation.
		///
		void pause();

		///
		/// Stop the animation. If you call play() the animation starts from the beginning.
		///
		void stop();

		///
		/// Controls if the animation is paused or not.
		///
		bool m_paused;

		///
		/// Current amount of time spent on the frame.
		///
		std::size_t m_time_spent_on_frame;

		///
		/// Currently playing animation.
		///
		std::string m_active_animation;

		///
		/// Stored animations.
		///
		std::unordered_map<std::string, qs::Animation> m_animations;
	};
} // namespace galaxy

#endif