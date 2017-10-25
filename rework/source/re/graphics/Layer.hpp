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

namespace re
{
	class Layer
	{
	public:
		///
		/// Constructor.
		///
		/// \param atlas Pointer to texture packer.
		/// \param defaultAlloc Amount of space reserved in containers to prevent constant reallocation.
		///
		Layer(unsigned int defaultAlloc = 20);
	
		///
		/// Destructor.
		///
		~Layer();

		///
		/// Render renderables on this layer.
		///
		void render();

		///
		/// Insert a sprite into this layer.
		///
		/// \param sc SpriteComponent to insert.
		///
		void insert(Renderable* rc);
		///
		/// Sort containers.
		///
		//void sort();
		
		///
		/// Remove data from containers.
		///
		void clean();

	private:
		unsigned int m_defaultAlloc;
		std::vector<Renderable*> m_renderables;
	};
}

#endif

/*
void Layer::sort()
{
	std::sort(m_sprites.begin(), m_sprites.end(), [](ex::Entity& a, ex::Entity& b)
	{
	return a.component<SpriteComponent>()->m_layer < b.component<SpriteComponent>()->m_layer;
	});

	std::sort(m_texts.begin(), m_texts.end(), [](ex::Entity& a, ex::Entity& b)
	{
	return a.component<TextComponent>()->m_layer < b.component<TextComponent>()->m_layer;
	});

	m_sprites.shrink_to_fit();
	m_texts.shrink_to_fit();
}
*/