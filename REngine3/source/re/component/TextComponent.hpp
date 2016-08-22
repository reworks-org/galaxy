//
//  TextComponent.hpp
//  REngine3
//
//  Created by reworks on 19/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_TEXTCOMPONENT_HPP_
#define RENGINE3_TEXTCOMPONENT_HPP_

#include <SFML/Graphics/Text.hpp>

#include "re/component/Component.hpp"
#include "re/services/vfs/sfmlphysfs.hpp"

namespace re
{
	class TextComponent : public Component
	{
	public:
		/*
		* IMPORTS: sol::table from lua script containing component data.
		* EXPORTS: none
		* PURPOSE: Set up the component.
		*/
		TextComponent(sol::table& table);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean up the component.
		*/
		~TextComponent() override;

		/*
		* IMPORTS: x and y position
		* EXPORTS: none
		* PURPOSE: Update component data.
		*/
		void Update(sf::Vector2f pos);

	public: 
		sf::Text m_text;

	private:
		sf::physfs fontData;
		sf::Font m_font;
	};
}

#endif