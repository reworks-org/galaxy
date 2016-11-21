//
//  AnimationComponent.hpp
//  REngine3
//
//  Created by reworks on 16/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//  Code modified from: https://github.com/miguelmartin75/anax/blob/master/examples/common/include/Components/AnimationComponent.hpp
//  See original file for details.

#ifndef RENGINE3_ANIMATIONCOMPONENT_HPP_
#define RENGINE3_ANIMATIONCOMPONENT_HPP_

#include <map>

#include <SFML/Graphics/Sprite.hpp>

#include "re/types/Component.hpp"

namespace re
{
	struct AnimationState
	{
		AnimationState();
		AnimationState(sol::table& table);

		/// The starting position of the animaton
		sf::Vector2u m_startPosition;

		/// The amount of frames in each direction
		sf::Vector2u m_frameAmount;

		/// The frame rate you wish to play the state at
		/// Set this to 0 if you wish to use the animation frame rate
		unsigned int m_frameRate;
	};

	class AnimationComponent : public Component
	{
		friend class AnimationSystem;
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Default Constructor.
		*/
		AnimationComponent();

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Destructor. Cleans up component.
		*/
		~AnimationComponent() override;

		/*
		* IMPORTS: sol::table from lua script containing component data.
		* EXPORTS: none
		* PURPOSE: Set up the component.
		*/
		void Init(sol::table& table) override;

		/*
		* IMPORTS: state to play
		* EXPORTS: none
		* PURPOSE: Play animation.
		*/
		void Play(const std::string& state);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Reset animation.
		*/
		void Reset();

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Pause animation.
		*/
		void Pause();

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Unpauses animation.
		*/
		void Unpause();
		
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Enable animation repeat.
		*/
		void EnableRepeat();

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Disable animation repeat.
		*/
		void DisableRepeat();

		/*
		* IMPORTS: stateName - animation state to change to.
		* EXPORTS: none
		* PURPOSE: Manually change animation.
		*/
		void ChangeAnimation(const std::string& stateName);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Stop animation.
		*/
		void Stop();

	private:
		bool m_isPlaying;
		bool m_repeat;
		std::string m_playingState;
		sf::Vector2u m_frameSize;
		std::map<std::string, AnimationState> m_states;
		sf::Vector2u m_currentFrame;
		double m_frameAccumulator;
	};
}

#endif