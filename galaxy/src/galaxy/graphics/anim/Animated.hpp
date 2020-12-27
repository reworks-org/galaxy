///
/// Animated.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_ANIM_ANIMATED_HPP_
#define GALAXY_GRAPHICS_ANIM_ANIMATED_HPP_

#include <robin_hood.h>

#include "galaxy/graphics/anim/Animation.hpp"

namespace galaxy
{
	namespace graphics
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
			/// Add an animation to the object.
			///
			/// \param name Name of the animation.
			/// \param args Variadic arguments to construct animation with.
			///
			template<typename... Args>
			void add_animation(std::string_view name, Args&&... args);

			///
			/// Update GL data.
			///
			/// \param dt DeltaTime.
			///
			virtual void update(const double dt) = 0;

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
			/// Get current animation.
			///
			/// \return Pointer to currently playing animation.
			///
			[[nodiscard]] Animation* get_cur_animation();

			///
			/// Get all animations.
			///
			/// \return Map of all animations.
			///
			[[nodiscard]] auto get_all_anims() -> const robin_hood::unordered_map<std::string, Animation>&;

		protected:
			///
			/// Current active animation.
			///
			Animation* m_active_anim;

			///
			/// Controls if the animation is paused or not.
			///
			bool m_paused;

			///
			/// Current amount of time spent on the frame.
			///
			double m_time_spent_on_frame;

			///
			/// Stored animations.
			///
			robin_hood::unordered_map<std::string, Animation> m_animations;
		};

		template<typename... Args>
		inline void Animated::add_animation(std::string_view name, Args&&... args)
		{
			m_animations.emplace(name, Animation {std::forward<Args>(args)...});
		}
	} // namespace graphics
} // namespace galaxy

#endif