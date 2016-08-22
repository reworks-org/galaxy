//
//  SpriteComponent.hpp
//  REngine3
//
//  Created by reworks on 12/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_SPRITECOMPONENT_HPP_
#define RENGINE3_SPRITECOMPONENT_HPP_

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Shader.hpp>

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
		sf::VertexArray m_vertices;
		sf::Transformable m_transform;
		sf::Texture m_texture;

		// Design oversight -> inherits from non-copyable, wrapping it in a pointer prevents a deleted constructor error.
		std::shared_ptr<sf::Shader> m_shader;
	};
}

#endif