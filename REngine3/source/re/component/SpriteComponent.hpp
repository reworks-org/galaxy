//
//  SpriteComponent.hpp
//  REngine3
//
//  Created by reworks on 12/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_SPRITECOMPONENT_HPP_
#define RENGINE3_SPRITECOMPONENT_HPP_

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "re/types/Component.hpp"
#include "re/services/vfs/sfmlphysfs.hpp"

namespace re
{
	class SpriteComponent : public Component, public sf::Sprite
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
		* EXPORTS: none
		* PURPOSE: Update the component.
		*/
		void Update(sf::Time dt) override;

		/*
		* IMPORTS: none
		* EXPORTS: sf::Shader
		* PURPOSE: Get the sprites shader.
		*/
		sf::Shader* Shader();

	private:
		/*
		* IMPORTS: texture - Name of the texture to use in the VFS.
		* EXPORTS: none
		* PURPOSE: Replaces sf::Sprites setTexture method. Stores the texture data in the class.
		*/
		void LoadTexture(const std::string& texture);

		/*
		* IMPORTS: vert, frag - Name of the shaders to use in the VFS.
		* EXPORTS: none
		* PURPOSE: Replaces sf::Sprites shader loading. Stores the shader data in the class.
		*/
		void LoadShader(const std::string& vert, const std::string& frag);

		/*
		* IMPORTS: RenderTarget & RenderStates
		* EXPORTS: none
		* PURPOSE: draw to target using states. See SFML documentation.
		*/
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	private:
		sf::physfs m_textureStream;
		sf::Texture m_texture;

		sf::physfs m_vertStream;
		sf::physfs m_fragStream;
		std::shared_ptr<sf::Shader> m_shader;

	public:
		unsigned long m_group;
	};
}

#endif