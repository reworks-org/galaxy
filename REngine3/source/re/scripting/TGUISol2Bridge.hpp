//
//  TGUISol2Bridge.hpp
//  REngine3
//
//  Created by reworks on 20/10/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_TGUISOL2BRIDGE_HPP_
#define RENGINE3_TGUISOL2BRIDGE_HPP_

#include <TGUI/TGUI.hpp>

#include "re/services/vfs/VFS.hpp"
#include "re/scripting/sol2/sol.hpp"
#include "re/graphics/FontManager.hpp"
#include "re/services/ServiceLocator.hpp"

namespace tgui
{
	/*
	* IMPORTS: Smart Pointer to your tgui::Theme object, and name of lua script.
	* EXPORTS: Smart Pointer to your widget.
	* PURPOSE: To use a lua script to create the object instead of hard-coding it.
	* NOTE: Does not connect a function, add to gui object or set a tooltip.
	*/
	inline Button::Ptr loadButtonWithScript(Theme::Ptr theme, const std::string& script)
	{
		sol::state lua;
		lua.script(re::Locator::Get<re::VFS>()->ToString(script));
		sol::table widget = lua.get<sol::table>("widgetButton");

		Button::Ptr button = theme->load("Button");
		button->setSize(widget.get<int>("w"), widget.get<int>("h"));
		button->setPosition(widget.get<int>("x"), widget.get<int>("y"));
		button->setText(widget.get<std::string>("text"));
		button->setFont(re::Locator::Get<re::FontManager>()->Get(widget.get<std::string>("font")));
		button->setOpacity(widget.get<float>("opacity"));
		button->setTextSize(widget.get<int>("fontSize"));

		return button;
	}

	/*
	* IMPORTS: Smart Pointer to your tgui::Theme object, and name of lua script.
	* EXPORTS: Smart Pointer to your widget.
	* PURPOSE: To use a lua script to create the object instead of hard-coding it.
	* NOTE: Does not add to gui object or set a tooltip.
	*/
	inline Label::Ptr loadLabelWithScript(Theme::Ptr theme, const std::string& script)
	{
		sol::state lua;
		lua.script(re::Locator::Get<re::VFS>()->ToString(script));
		sol::table widget = lua.get<sol::table>("widgetLabel");

		Label::Ptr label = theme->load("Label");
		label->setAutoSize(widget.get<bool>("autoSize"));
		label->setFont(re::Locator::Get<re::FontManager>()->Get(widget.get<std::string>("font")));
		label->setOpacity(widget.get<float>("opacity"));
		label->setPosition(widget.get<int>("x"), widget.get<int>("y"));
		label->setSize(widget.get<int>("w"), widget.get<int>("h"));
		label->setText(widget.get<std::string>("text"));
		label->setTextSize(widget.get<int>("fontSize"));

		sol::table colour = widget.get<sol::table>("colour");
		tgui::Color col(colour.get<sf::Uint8>("r"), colour.get<sf::Uint8>("g"), colour.get<sf::Uint8>("b"), colour.get<sf::Uint8>("a"));
		label->setTextColor(col);

		// left is 0, center is 1, right is 2.
		switch (widget.get<int>("horizontalAlignment"))
		{
		case 0:
			label->setHorizontalAlignment(Label::HorizontalAlignment::Left);
			break;

		case 1:
			label->setHorizontalAlignment(Label::HorizontalAlignment::Center);
			break;

		case 2:
			label->setHorizontalAlignment(Label::HorizontalAlignment::Right);
			break;
		}

		// top is 0, center is 1, bottom is 2.
		switch (widget.get<int>("verticalAlignment"))
		{
		case 0:
			label->setVerticalAlignment(Label::VerticalAlignment::Top);
			break;

		case 1:
			label->setVerticalAlignment(Label::VerticalAlignment::Center);
			break;

		case 2:
			label->setVerticalAlignment(Label::VerticalAlignment::Bottom);
			break;
		}

		/*
		0 = Regular,
		1 = Bold,
		2 = Italic,
		3 = Underlined,
		4 = StrikeTrough

		See <SFML/Graphics/Text.hpp>
		*/
		int style = widget.get<int>("style");

		switch (style)
		{
		case 0:
			label->setTextStyle(sf::Text::Regular);
			break;

		case 1:
			label->setTextStyle(sf::Text::Bold);
			break;

		case 2:
			label->setTextStyle(sf::Text::Italic);
			break;

		case 3:
			label->setTextStyle(sf::Text::Underlined);
			break;

		case 4:
			label->setTextStyle(sf::Text::StrikeThrough);
			break;
		}

		return label;
	}
}

#endif
