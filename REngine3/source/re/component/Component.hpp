//
//  Component.hpp
//  REngine3
//
//  Created by reworks on 18/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_COMPONENT_HPP_
#define RENGINE3_COMPONENT_HPP_

#include "re/deps/sol2/sol.hpp"

namespace re
{
	class Component
	{
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Abstract virtual destructor for engine systems.
		*/
		virtual inline ~Component() {}
	};
}

#endif