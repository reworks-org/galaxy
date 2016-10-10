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
		m_animated.clear();
	}

	void Group::AddDrawable(sf::Uint64 id, std::shared_ptr<sf::Drawable> drawable)
	{
		m_drawable.emplace(id, drawable);
	}

	void Group::AddAnimated(sf::Uint64 id, std::shared_ptr<Animated> animated)
	{
		m_drawable.emplace(id, animated);
		m_animated.emplace(id, animated);
	}

	void Group::Update(sf::Time dt)
	{
		for (auto& it : m_animated)
		{
			it.second->Update(dt);
		}
	}

	void Group::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		for (auto& it : m_drawable)
		{
			target.draw(*(it.second.get()), states);
		}
	}

	std::map<sf::Uint64, std::shared_ptr<Animated>>& Group::GetAnimatedMap()
	{
		return m_animated;
	}

	std::map<sf::Uint64, std::shared_ptr<sf::Drawable>>& Group::GetDrawableMap()
	{
		return m_drawable;
	}
}