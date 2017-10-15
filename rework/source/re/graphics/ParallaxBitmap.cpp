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
	ParallaxBitmap::ParallaxBitmap(const std::string& atlasName, float x, float y, float speed, int layer)
		:m_atlasName(atlasName), m_x(x), m_y(y), m_speed(speed), m_layer(layer)
	{
	}

	ParallaxBitmap::~ParallaxBitmap()
	{
	}
}