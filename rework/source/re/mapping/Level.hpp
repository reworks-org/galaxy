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

// we can then update the animations aswell.
// each layer should be a bitmap so we can instert it into layer render engine
// movesystem needs to grab tile information in order to make actions occur
// create following animated_tile struct:
/*
struct AnimatedTile
{
	unsigned int gid;
	int dest_x;
	int dest_y;
	int cur_frame;

};

or something similar.
Refer to tmx map docs.
*/

// https://github.com/baylej/tmx/blob/master/examples/dumper/dumper.c
// https://github.com/baylej/tmx/blob/master/examples/allegro/allegro.c

namespace re
{
	class Level
	{
	public:
		///
		/// Level constructor.
		///
		/// \param script Lua script outlining level data.
		///
		Level(const std::string& script);

		///
		/// Destructor.
		///
		virtual ~Level();

		///
		/// Return current map bounds.
		///
		/// \return Rect<float, int> CONST
		///
		const Rect<float, int>& getBounds() const;

	protected:
		tmx_map* m_map;
		Rect<float, int> m_bounds;
	};
}

#endif