//
//  AnimationComponent.hpp
//  rework
//
//  Created by reworks on 16/08/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_ANIMATIONCOMPONENT_HPP_
#define REWORK_ANIMATIONCOMPONENT_HPP_

#include "sol2/sol.hpp"
#include "re/math/Rect.hpp"

namespace re
{
	class AnimationComponent
	{
	public:
		///
		/// Constructor.
		///
		/// \param table sol::table containing data.
		///
		AnimationComponent(sol::table& table);

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
		/// Check if animation is paused.
		///
		bool isPaused() const;

		///
		/// Calls imgui debug functions. Don't call this, done for you by debugmanager.
		///
		void debug();

	private:
		bool m_isPaused;
        
		float m_frameTime;
		float m_currentTime;
        
		std::size_t m_currentFrame;
		std::string m_activeAnimation;
		std::unordered_map<std::string, std::vector<Rect<int>>> m_animations;

	public:
		bool m_isLooped;
		Rect<int> m_currentFrameRect;
	};
}

#endif