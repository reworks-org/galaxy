//
//  AnimatedSpriteComponent.hpp
//  REngine3
//
//  Created by reworks on 16/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_ANIMATEDSPRITECOMPONENT_HPP_
#define RENGINE3_ANIMATEDSPRITECOMPONENT_HPP_

#include "re/component/Component.hpp"

namespace re
{
	class AnimatedSpriteComponent : public Component
	{
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Default constructor. Sets up component.
		*/
		AnimatedSpriteComponent(sol::table& table);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Destructor. Cleans up component.
		*/
		~AnimatedSpriteComponent() override;

	public:
	};
}

#endif