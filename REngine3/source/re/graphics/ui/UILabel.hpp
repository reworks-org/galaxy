//
//  UILabel.hpp
//  REngine3
//
//  Created by reworks on 12/09/2016.
//  Copyright © 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_UILABEL_HPP_
#define RENGINE3_UILABEL_HPP_

#include <SFML/Graphics/Text.hpp>

#include "re/graphics/ui/UIComponent.hpp"

namespace re
{
	class UILabel : public UIComponent
	{
	public:
		UILabel(const std::string& script);
		~UILabel() override;

		/*
		* IMPORTS: x, y integers
		* EXPORTS: none
		* PURPOSE: Set the offset of the label relative to the screen.
		*/
		void SetOffset(int x, int y) override;
		
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Processes label's events.
		*/
		void Event() override;

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Updates label.
		*/
		void Update() override;

	private:
		/*
		* IMPORTS: RenderTarget & RenderStates
		* EXPORTS: none
		* PURPOSE: draw to target using states. See SFML documentation.
		*/
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		
	private:
		sf::Font m_font;
		sf::physfs m_fontStream;

		bool m_hasDropShadow;
		float m_dropShadowOffset;

	public:
		sf::Text m_text;
	};
}

#endif