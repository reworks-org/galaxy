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

	void Group::AddDrawable(std::shared_ptr<sf::Drawable> drawable)
	{
		m_drawable.push_back(drawable);
	}

	void Group::AddAnimated(std::shared_ptr<Animated> animated)
	{
		m_animated.push_back(animated);
	}

	void Group::Update(sf::Time dt)
	{
		for (auto& v : m_animated)
		{
			v->Update(dt);
		}
	}

	void Group::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		for (auto& v : m_drawable)
		{
			target.draw(*v.get(), states);
		}

		for (auto& v : m_animated)
		{
			target.draw(*v.get(), states);
		}
	}
}