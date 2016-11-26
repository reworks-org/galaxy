//
//  AnimationComponent.hpp
//  REngine3
//
//  Created by reworks on 16/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//
// Code adapted from here: https://github.com/SFML/SFML/wiki/Source:-AnimatedSprite

#ifndef RENGINE3_ANIMATEDSPRITECOMPONENT_HPP_
#define RENGINE3_ANIMATEDSPRITECOMPONENT_HPP_

#include "re/types/Component.hpp"

#include "re/graphics/Animation.hpp"

namespace re
{
	class AnimationComponent : public Component
	{
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
		* IMPORTS: animantion name in string
		* EXPORTS: none
		* PURPOSE: Set the components current active animation.
		*/
		void ChangeAnimation(const std::string& animation);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Play the current animation.
		*/
		void Play();

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Stop the current animation.
		*/
		void Stop();

	private:
		float m_speed;
		bool m_isPlaying;
		std::string m_activeAnimation;
		std::unordered_map<std::string, Animation> m_animations;
	};
}

#endif