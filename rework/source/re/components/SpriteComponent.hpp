//
//  SpriteComponent.hpp
//  REngine3
//
//  Created by reworks on 12/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_SPRITECOMPONENT_HPP_
#define RENGINE3_SPRITECOMPONENT_HPP_

#include "re/types/Component.hpp"

namespace re
{
	class SpriteComponent : public Component
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
		* IMPORTS: Name of a single texture in the vfs. DOES NOT WORK ON SPRITESHEETS! USE LOADTEXTURE() instead.
		* EXPORTS: none
		* PURPOSE: Load a single texture from vfs.
		*/
		void loadSingleTexture(const std::string& textureName);

		/*
		* IMPORTS: sol::table from lua script containing component data, and optionally specify a texture.
		* EXPORTS: none
		* PURPOSE: Load the texture.
		*/
		void loadTexture(sol::table& table, const std::string& texture = "");

		/*
		* IMPORTS: lua table and entity name
		* EXPORTS: Whether or not to save the changed table data.
		* PURPOSE: debug component, change data, etc.
		*/
		void debugFunction(sol::table& table, const std::string& curEntityName) override;

	public:
		sf::Uint32 m_group;
		sf::Sprite m_sprite;

	private:
		sf::Texture m_texture;
	};
}

#endif
