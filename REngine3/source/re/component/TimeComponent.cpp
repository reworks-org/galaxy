//
//  TimeComponent.cpp
//  REngine3
//
//  Created by reworks on 12/10/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <SFML/Graphics/RenderTarget.hpp>

#include "re/utility/Time.hpp"
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
		m_time.setPosition(table.get<float>("x"), table.get<float>("y"));
		m_group = table.get<unsigned long>("group");

		m_time.setFont(Locator::Get<FontManager>()->Get(table.get<std::string>("font")));

		m_time.setCharacterSize(table.get<int>("size"));

		sol::table colour = table.get<sol::table>("colour");

		sf::Color col;
		col.r = colour.get<sf::Uint8>("r");
		col.g = colour.get<sf::Uint8>("g");
		col.b = colour.get<sf::Uint8>("b");
		col.a = colour.get<sf::Uint8>("a");
		m_time.setFillColor(col);

		m_time.setStyle(sf::Text::Regular);
	}

	void TimeComponent::Update(sf::Time dt)
	{
		std::string temp = Time::GetCurrentTimeAndDate();

		temp.erase(temp.length() - 8);

		if (std::stoi(temp.substr(11, 2)) >= 12) {
			temp.append(" PM");
		}
		else {
			temp.append(" AM");
		}

		temp = temp.substr(11, 8);

		m_time.setString(temp);
	}

	void TimeComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(m_time);
	}
}