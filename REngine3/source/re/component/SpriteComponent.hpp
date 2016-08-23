//
//  SpriteComponent.hpp
//  REngine3
//
//  Created by reworks on 12/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_SPRITECOMPONENT_HPP_
#define RENGINE3_SPRITECOMPONENT_HPP_

#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "re/component/Component.hpp"
#include "re/services/vfs/sfmlphysfs.hpp"

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
		
		/*
		* IMPORTS: none
		* EXPORTS: sf::Shader
		* PURPOSE: Get the sprites shader.
		*/
		sf::Shader* Shader();

	public:	
		unsigned long m_group;

		sf::Sprite m_sprite;

	private:
		sf::physfs textureData;
		sf::Texture m_texture;

		sf::physfs shaderData;
		std::shared_ptr<sf::Shader> m_shader;
	};
}

#endif