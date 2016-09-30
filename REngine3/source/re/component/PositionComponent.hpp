//
//  PositionComponent.hpp
//  REngine3
//
//  Created by reworks on 30/09/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_POSITIONCOMPONENT_HPP_
#define RENGINE3_POSITIONCOMPONENT_HPP_

#include "re/types/Component.hpp"

namespace re
{
	class PositionComponent : public Component
	{
	public:
		PositionComponent(sol::table& table);
		~PositionComponent();

	public:
		float x_pos;
		float y_pos;
	};
}

#endif