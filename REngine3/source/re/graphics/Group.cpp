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

	void Group::AddDrawable(const std::string& name, std::shared_ptr<sf::Drawable> drawable, std::shared_ptr<sf::Transformable> transformable)
	{
		auto it = m_drawable.find(name);
		if (it != m_drawable.end())
		{
			m_drawable[name].push_back(std::make_pair(drawable, transformable));
		}
		else
		{
			m_drawable.emplace(name, RenderableContainer());
			m_drawable[name].push_back(std::make_pair(drawable, transformable));
		}
	}

	void Group::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		sf::RenderStates copy = states;
		for (auto& it : m_drawable)
		{
			for (auto& v : it.second)
			{
				copy.transform *= v.second->getTransform();
				target.draw(*(v.first), copy);
				copy = states;
			}
		}
	}

	std::map<std::string, RenderableContainer>& Group::GetDrawableMap()
	{
		return m_drawable;
	}
}