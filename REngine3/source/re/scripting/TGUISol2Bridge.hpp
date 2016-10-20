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
	tgui::Button::Ptr load(Theme::Ptr theme, std::string& script)
	{
		sol::state lua;
		lua.script(re::Locator::Get<re::VFS>()->ToString(script));
		sol::table widget = lua.get<sol::table>("widget");

		tgui::Button::Ptr button = theme->load(lua.get<std::string>("class"));
		button->setSize(lua.get<int>("w"), lua.get<int>("h"));
		button->setPosition(lua.get<int>("x"), lua.get<int>("y"));
		button->setText(lua.get<std::string>("text"));
		button->setFont(re::Locator::Get<re::FontManager>()->Get(lua.get<std::string>("font")));
		button->setOpacity(lua.get<float>("opacity"));
		button->setTextSize(lua.get<std::string>("fontSize"));

		return button;
	}


}

#endif