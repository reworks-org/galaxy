//
//  LayerComponent.hpp
//  rework
//
//  Created by reworks on 17/10/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_LAYERCOMPONENT_HPP_
#define REWORK_LAYERCOMPONENT_HPP_

#include "sol2/sol.hpp"

namespace re
{
	struct LayerComponent
	{
		LayerComponent(sol::table& table);

		std::size_t m_layer;
	};
}

#endif