//
//  Layer.hpp
//  rework
//
//  Created by reworks on 11/08/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_LAYER_HPP_
#define REWORK_LAYER_HPP_

#include "entityx/entityx.h"
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

		void insert(ex::Entity& e);

		void sort();

	private:
		std::vector<ex::Entity> m_sprites;
		std::vector<ex::Entity> m_texts;
		TexturePacker* m_atlas;
	};
}

#endif