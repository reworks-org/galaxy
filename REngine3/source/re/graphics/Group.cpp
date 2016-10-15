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
		auto it = m_drawable.find(id);
		if (it != m_drawable.end())
		{
			m_drawable[id].push_back(drawable);
		}
		else
		{
			m_drawable.emplace(id, std::vector<std::shared_ptr<sf::Drawable>>());
			m_drawable[id].push_back(drawable);
		}
	}

	void Group::AddAnimated(sf::Uint64 id, std::shared_ptr<Animated> animated)
	{
		auto it = m_drawable.find(id);
		if (it != m_drawable.end())
		{
			m_drawable[id].push_back(animated);
		}
		else
		{
			m_drawable.emplace(id, std::vector<std::shared_ptr<sf::Drawable>>());
			m_drawable[id].push_back(animated);
		}

		auto it2 = m_animated.find(id);
		if (it2 != m_animated.end())
		{
			m_animated[id].push_back(animated);
		}
		else
		{
			m_animated.emplace(id, std::vector<std::shared_ptr<Animated>>());
			m_animated[id].push_back(animated);
		}
	}

	void Group::Update(sf::Time dt)
	{
		for (auto& it : m_animated)
		{
			for (auto& v : it.second)
			{
				v->Update(dt);
			}
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

	std::map<sf::Uint64, std::vector<std::shared_ptr<Animated>>>& Group::GetAnimatedMap()
	{
		return m_animated;
	}

	std::map<sf::Uint64, std::vector<std::shared_ptr<sf::Drawable>>>& Group::GetDrawableMap()
	{
		return m_drawable;
	}
}