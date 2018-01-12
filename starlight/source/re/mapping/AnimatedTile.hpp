//
//  AnimatedTile.hpp
//  rework
//
//  Created by reworks on 05/12/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_ANIMATEDTILE_HPP_
#define REWORK_ANIMATEDTILE_HPP_

namespace re
{
	struct AnimatedTile
	{
		unsigned int gid;
		unsigned int cur_frame;
		int dest_x;
		int dest_y;
		float elapsedTime;
	};
}

#endif