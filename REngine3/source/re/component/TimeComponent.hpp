//
//  TimeComponent.hpp
//  REngine3
//
//  Created by reworks on 12/10/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_TIMECOMPONENT_HPP_
#define RENGINE3_TIMECOMPONENT_HPP_

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Drawable.hpp>

#include "re/types/Component.hpp"

namespace re
{
	class TimeComponent : public Component, public sf::Drawable
	{
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Default Constructor.
		*/
		TimeComponent();

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean up the component.
		*/
		~TimeComponent() override;

		/*
		* IMPORTS: sol::table from lua script containing component data.
		* EXPORTS: none
		* PURPOSE: Set up the component.
		*/
		void Init(sol::table& table) override;

		/*
		* IMPORTS: A change in position if nessessary. Delta time aswell.
		* EXPORTS: none
		* PURPOSE: Update the component.
		*/
		void Update(sf::Time dt, float x, float y) override;

	private:
		/*
		* IMPORTS: RenderTarget & RenderStates
		* EXPORTS: none
		* PURPOSE: draw to target using states. See SFML documentation.
		*/
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	public:
		unsigned long m_group;

	private:
		sf::Text m_time;
	};
}

#endif