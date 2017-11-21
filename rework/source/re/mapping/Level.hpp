//
//  Level.hpp
//  rework
//
//  Created by reworks on 21/11/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_LEVEL_HPP_
#define REWORK_LEVEL_HPP_

#include "re/mapping/TMXMap.hpp"

namespace re
{
	class Level
	{
	public:
		Level(const std::string& map);
		~Level();

	private:
		TMXMap m_map;
		Rect<float, int> m_dimensions;
	};
}

#endif