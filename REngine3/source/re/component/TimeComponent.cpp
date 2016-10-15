//
//  TimeComponent.cpp
//  REngine3
//
//  Created by reworks on 12/10/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "re/utils/Time.hpp"
#include "re/services/vfs/VFS.hpp"
#include "re/graphics/FontManager.hpp"
#include "re/services/ServiceLocator.hpp"

#include "TimeComponent.hpp"

namespace re
{
	TimeComponent::TimeComponent()
	{
	}

	TimeComponent::~TimeComponent()
	{
	}

	void TimeComponent::Init(sol::table& table)
	{
		setPosition(table.get<float>("x"), table.get<float>("y"));
		m_group = table.get<unsigned long>("group");

		setFont(Locator::Get<FontManager>()->Get(table.get<std::string>("font")));

		setCharacterSize(table.get<int>("size"));

		sol::table colour = table.get<sol::table>("colour");

		sf::Color col;
		col.r = colour.get<sf::Uint8>("r");
		col.g = colour.get<sf::Uint8>("g");
		col.b = colour.get<sf::Uint8>("b");
		col.a = colour.get<sf::Uint8>("a");
		setFillColor(col);

		setStyle(sf::Text::Regular);
	}

	void TimeComponent::Update()
	{
		std::string temp = GetCurrentTimeAndDate();

		temp.erase(temp.length() - 8);

		if (std::stoi(temp.substr(11, 2)) >= 12) {
			temp.append(" PM");
		}
		else {
			temp.append(" AM");
		}

		temp = temp.substr(11, 8);

		setString(temp);
	}
}