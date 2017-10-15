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
		///
		/// Set up a new parallax bitmap.
		///
		/// \param atlasName Name of the bitmap in the texture atlas.
		/// \param x X position.
		/// \param y Y position.
		/// \param speed Speed of the bitmap to scroll on the screen.
		/// \param layer Layer to draw on.
		///
		ParallaxBitmap(const std::string& atlasName, float x, float y, float speed, int layer);
		
		///
		/// Destructor.
		///
		~ParallaxBitmap();

	private:
		std::string m_atlasName;

		float m_x;
		float m_y;
		float m_speed;
		int m_layer;
	};
}

#endif