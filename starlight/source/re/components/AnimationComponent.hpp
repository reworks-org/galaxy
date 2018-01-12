///
///  AnimationComponent.hpp
///  starlight
///
///  Created by reworks on 16/08/2016.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_ANIMATIONCOMPONENT_HPP_
#define STARLIGHT_ANIMATIONCOMPONENT_HPP_

#include <unordered_map>

#include "re/graphics/Animation.hpp"

namespace sl
{
	class AnimationComponent final
	{
	public:
		///
		/// Constructor.
		///
		/// \param table sol::table containing data.
		///
		AnimationComponent(const sol::table& table);

		///
		/// Move Constructor.
		///
		AnimationComponent(AnimationComponent&&) = default;

		///
		/// Destructor.
		///
		~AnimationComponent();

		///
		/// Change the current animation.
		///
		/// \param animation The name of the animation type to change to as defined in lua file. E.g. "walking" -> "running"
		///
		void changeAnimation(const std::string& animation);

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
		/// Rest animation frames to beginning.
		///
		void stop();

		///
		/// Calls imgui debug functions. Don't call this, done for you by debugmanager.
		///
		void debug();

	public:
		bool m_isPaused;
		double m_currentFrameTime;
		std::string m_activeAnimation;
		std::unordered_map<std::string, Animation> m_animations;

	private:
		///
		/// Default constructor.
		/// Deleted.
		///
		AnimationComponent() = delete;

		///
		/// Copy Constructor.
		/// Deleted.
		///
		AnimationComponent(const AnimationComponent&) = delete;
	};
}

#endif