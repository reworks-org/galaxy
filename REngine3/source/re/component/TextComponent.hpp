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

#include "re/types/Component.hpp"

namespace re
{
	class TextComponent : public Component, public sf::Text
	{
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Default Constructor.
		*/
		TextComponent();

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean up the component.
		*/
		~TextComponent() override;

		/*
		* IMPORTS: sol::table from lua script containing component data.
		* EXPORTS: none
		* PURPOSE: Set up the component.
		*/
		void Init(sol::table& table) override;

	public:
		unsigned long m_group;
	};
}

#endif