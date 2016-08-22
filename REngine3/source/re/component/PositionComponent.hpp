//
//  PositionComponent.hpp
//  REngine3
//
//  Created by reworks on 1/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_POSITIONCOMPONENT_HPP_
#define RENGINE3_POSITIONCOMPONENT_HPP_

#include "re/component/Component.hpp"

namespace re
{
	class PositionComponent : public Component
	{
	public:
		/*
		* IMPORTS: sol::table containing position component.
		* EXPORTS: none
		* PURPOSE: Construct PositionComponent.
		*/
		PositionComponent(sol::table& table);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean up the component.
		*/
		~PositionComponent() override;

	public:
		unsigned long m_group;

		float x;
		float y;
		float w;
		float h;
	};
}

#endif