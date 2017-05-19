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
#include <SFML/Graphics/Texture.hpp>

#include "re/types/Component.hpp"
#include "re/services/vfs/sfmlphysfs.hpp"

namespace re
{
	class SpriteComponent : public Component, public sf::Sprite
	{
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Default Constructor.
		*/
		SpriteComponent();

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean up the component.
		*/
		~SpriteComponent() override;

		/*
		* IMPORTS: sol::table from lua script containing component data.
		* EXPORTS: none
		* PURPOSE: Set up the component.
		*/
		void init(sol::table& table) override;

		/*
		* IMPORTS: sol::table from lua script containing component data, and optionally specify a texture.
		* EXPORTS: none
		* PURPOSE: Load the texture.
		*/
		void loadTexture(sol::table& table, const std::string& texture = "");

		/*
		* IMPORTS: none
		* EXPORTS: Whether or not to save the changed table data.
		* PURPOSE: debug component, change data, etc.
		* NOTE: INPUTTING TEXT CANNOT EXCEED 256 CHARACTERS! THIS ALSO ASSUMES YOU INPUT THE CORRECT STRING! THIS IS NOT MEANT FOR USERS! DEVS ONLY!
		*/
		bool debugFunction(sol::table& table) override;

	private:
		/*
		* IMPORTS: RenderTarget & RenderStates
		* EXPORTS: none
		* PURPOSE: draw to target using states. See SFML documentation.
		*/
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	public:
		sf::Uint32 m_group;

	private:
		sf::Texture m_texture;
		sf::physfs m_textureStream;
	};
}

#endif
