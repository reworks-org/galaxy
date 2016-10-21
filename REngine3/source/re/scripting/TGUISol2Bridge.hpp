//
//  TGUISol2Bridge.hpp
//  REngine3
//
//  Created by reworks on 20/10/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_TGUISOL2BRIDGE_HPP_
#define RENGINE3_TGUISOL2BRIDGE_HPP_

#include <memory>

#include <TGUI/TGUI.hpp>

#include "re/deps/sol2/sol.hpp"
#include "re/services/vfs/VFS.hpp"
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
	inline Button::Ptr load(Theme::Ptr theme, const std::string& script)
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
}

#endif