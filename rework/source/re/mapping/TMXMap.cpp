//
//  TMXMap.cpp
//  rework
//
//  Created by reworks on 17/11/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include "TMXMap.hpp"

namespace re
{
	TMXMap::TMXMap(const std::string& map)
	{
		// grab tiles between camera coords
		// in order to only render those
		// since we need to refresh the map every time we update the quadtree
		// we can then update the animations aswell. no need to worry about loading
		// static map once.
		// unless we only render a subsection of an al_bitmap.
		// might be a better idea?
		// we shall see.
		// each layer should be a bitmap so we can instert it into layer render engine
		// movesystem needs to grab tile information in order to make actions occur
	}

	TMXMap::~TMXMap()
	{
		tmx_map_free(m_map);
	}

	Rect<int>& TMXMap::getDimensions() const
	{
		return m_dimensions;
	}
}