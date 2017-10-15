//
//  ParallaxBitmap.cpp
//  rework
//
//  Created by reworks on 29/08/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include "ParallaxBitmap.hpp"

namespace re
{
	ParallaxBitmap::ParallaxBitmap(const std::string& atlasName, float x, float y, float velx, float vely, float dirx, float diry)
		:m_atlasName(atlasName), m_x(x), m_y(y), m_velX(velx), m_velY(vely), m_dirX(dirx), m_dirY(diry)
	{
	}

	ParallaxBitmap::~ParallaxBitmap()
	{
	}
}