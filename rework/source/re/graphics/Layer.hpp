//
//  Layer.hpp
//  rework
//
//  Created by reworks on 11/08/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_LAYER_HPP_
#define REWORK_LAYER_HPP_

#include "re/graphics/TexturePacker.hpp"

namespace re
{
	class Layer
	{
	public:
		///
		/// Constructor.
		///
		/// \param Atlas pointer to texture packer.
		///
		Layer(TexturePacker* atlas);

		///
		/// Draw sprites on this layer.
		///
		void draw();

	private:
		TexturePacker* m_atlas;
	};
}

#endif