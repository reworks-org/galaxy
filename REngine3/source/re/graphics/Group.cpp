//
//  Group.cpp
//  REngine3
//
//  Created by reworks on 11/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "Group.hpp"

namespace re
{
	Group::~Group()
	{
		m_drawable.clear();
	}

	void Group::AddDrawable(const std::string& name, std::shared_ptr<sf::Drawable> drawable)
	{
		auto it = m_drawable.find(name);
		if (it != m_drawable.end())
		{
			m_drawable[name].push_back(drawable);
		}
		else
		{
			m_drawable.emplace(name, std::vector<std::shared_ptr<sf::Drawable>>());
			m_drawable[name].push_back(drawable);
		}
	}

	void Group::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		for (auto& it : m_drawable)
		{
			for (auto& v : it.second)
			{
				target.draw(*(v), states);
			}
		}
	}

	std::map<std::string, std::vector<std::shared_ptr<sf::Drawable>>>& Group::GetDrawableMap()
	{
		return m_drawable;
	}
}