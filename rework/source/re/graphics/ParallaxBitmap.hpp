//
//  ParallaxBitmap.hpp
//  rework
//
//  Created by reworks on 29/08/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_PARALLAXBITMAP_HPP_
#define REWORK_PARALLAXBITMAP_HPP_

#include <string>

namespace re
{
	class ParallaxBitmap
	{
	public:
		ParallaxBitmap(const std::string& atlasName, float x, float y, float velx, float vely, float dirx, float diry);
		~ParallaxBitmap();

	private:
		std::string m_atlasName;

		float m_x;
		float m_y;
		float m_velX;
		float m_velY;
		float m_dirX;
		float m_dirY;
	};
}

#endif