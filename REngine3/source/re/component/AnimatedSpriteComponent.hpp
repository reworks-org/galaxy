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
		AnimatedSpriteComponent(sol::table& table);
		~AnimatedSpriteComponent() override;

	public:

	};
}

#endif