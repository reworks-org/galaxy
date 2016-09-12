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

	void Group::AddDrawable(std::shared_ptr<TextComponent> tc)
	{
		m_drawable.push_back(tc);
	}

	void Group::AddDrawable(std::shared_ptr<SpriteComponent> sc)
	{
		m_drawable.push_back(sc);
	}

	void Group::AddDrawable(std::shared_ptr<AnimatedSpriteComponent> ac)
	{
		// m_drawable.push_back(ac);
	}

	void Group::Update()
	{
	}

	void Group::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		for (auto& v : m_drawable)
		{
			target.draw(*v.get(), states);
		}
	}
}