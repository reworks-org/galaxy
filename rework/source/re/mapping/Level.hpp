//
//  Level.hpp
//  rework
//
//  Created by reworks on 21/11/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_LEVEL_HPP_
#define REWORK_LEVEL_HPP_

#include <vector>

#include "re/math/Rect.hpp"
#include "re/mapping/TMXHelper.hpp"

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
// new idea: break up levels into seperate components to be processed by a level system! eggcelent idea! :D

namespace re
{
	class Level
	{
	public:
		Level(const std::string& script);
		~Level();

		///
		/// Return current map dimensions.
		///
		/// \return Rect<int> CONST
		///
		Rect<int>& getDimensions() const;

	private:
		tmx_map* m_map;
		Rect<float, int> m_dimensions;
		std::vector<ALLEGRO_BITMAP*> m_layers;
	};
}

#endif