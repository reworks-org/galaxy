//
//  SpriteComponent.hpp
//  REngine3
//
//  Created by reworks on 12/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_SPRITECOMPONENT_HPP_
#define RENGINE3_SPRITECOMPONENT_HPP_

#include <SFML/Graphics.hpp>

#include "re/component/Component.hpp"

namespace re
{
	class SpriteComponent : public Component
	{
	public:
		/*
		* IMPORTS: sol::table from lua script containing component data.
		* EXPORTS: none
		* PURPOSE: Set up the component.
		*/
		SpriteComponent(sol::table& table);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean up the component.
		*/
		~SpriteComponent() override;

		void Update(sf::Vector2f pos);

	public:
		unsigned long m_group;
		
		sf::VertexArray m_vertices;
		sf::Transformable m_transform;
		sf::Texture m_texture;
		sf::Shader m_shader;
	};
}

#endif