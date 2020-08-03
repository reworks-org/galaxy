///
/// Animated.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_ANIMATED_HPP_
#define QUASAR_ANIMATED_HPP_

#include <unordered_map>

#include <protostar/async/ProtectedArithmetic.hpp>

#include "qs/anim/Animation.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// Allows for a class to be animated.
	///
	class Animated
	{
	public:
		///
		/// Constructor.
		///
		Animated();

		///
		/// Virtual destructor.
		///
		virtual ~Animated();

		///
		/// Update GL data.
		///
		/// \param dt DeltaTime.
		///
		virtual void update(pr::ProtectedDouble* dt) = 0;

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
		void pause() noexcept;

		///
		/// Stop the animation. If you call play() the animation starts from the beginning.
		///
		void stop() noexcept;

	protected:
		///
		/// Current active animation.
		///
		qs::Animation* m_active_anim;

		///
		/// Controls if the animation is paused or not.
		///
		bool m_paused;

		///
		/// Current amount of time spent on the frame.
		///
		std::size_t m_time_spent_on_frame;

		///
		/// Stored animations.
		///
		std::unordered_map<std::string, qs::Animation> m_animations;
	};
} // namespace qs

#endif