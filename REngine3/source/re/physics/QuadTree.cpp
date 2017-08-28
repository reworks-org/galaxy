//
//  QuadTree.cpp
//  REngine3
//
//  Created by reworks on 28/08/2017.
//  Code ported from:
//  https://gamedevelopment.tutsplus.com/tutorials/quick-tip-use-quadtrees-to-detect-likely-collisions-in-2d-space--gamedev-374

#include "re/component/TransformComponent.hpp"
#include "re/component/CollisionComponent.hpp"

#include "QuadTree.hpp"

namespace re
{
	QuadTree::QuadTree(int level, sf::Rect<int> bounds, int maxLevels, int maxObjects)
	{
		m_level = level;
		m_bounds.height = bounds.height;
		m_bounds.left = bounds.left;
		m_bounds.top = bounds.top;
		m_bounds.width = bounds.width;

		m_maxLevels = maxLevels;
		m_maxObjects = maxObjects;
	}

	void QuadTree::clear()
	{
		m_objects.clear();

		for (auto& elem : m_nodes)
		{
			elem.clear();
		}
	}

	void QuadTree::insert(Entity* entity)
	{
		auto cc = entity->get<CollisionComponent>();
		auto tc = entity->get<TransformComponent>();

		sf::Rect<int> rect(tc->getPosition().x, tc->getPosition().y, cc->m_width, cc->m_height);
		int index = getIndex(rect);

		if (index != -1)
		{
			nodes[index].insert(entity);

			return;
		}

		m_objects.push_back(entity);

		if (m_objects.size() > m_maxObjects && m_level < m_maxLevels)
		{

}
	}

	void QuadTree::split()
	{
		int subWidth = (m_bounds.width / 2);
		int subHeight = (m_bounds.height / 2);
		int x = m_bounds.left;
		int y = m_bounds.top;
		
		m_nodes[0] = new QuadTree(m_level + 1, sf::Rect<int>(x + subWidth, y, subWidth, subHeight), m_maxLevels, m_maxObjects);
		m_nodes[1] = new QuadTree(m_level + 1, sf::Rect<int>(x, y, subWidth, subHeight), m_maxLevels, m_maxObjects);
		m_nodes[2] = new QuadTree(m_level + 1, sf::Rect<int>(x, y + subHeight, subWidth, subHeight), m_maxLevels, m_maxObjects);
		m_nodes[3] = new QuadTree(m_level + 1, sf::Rect<int>(x + subWidth, y + subHeight, subWidth, subHeight), m_maxLevels, m_maxObjects);
	}

	int QuadTree::getIndex(sf::Rect<int>& rect)
	{
		int index = -1;
		double verticalMidpoint = m_bounds.left + (m_bounds.width / 2);
		double horizontalMidpoint = m_bounds.top + (m_bounds.height / 2);

		bool topQuadrant = (rect.top < horizontalMidpoint && rect.top + rect.height < horizontalMidpoint);
		bool bottomQuadrant = (rect.top > horizontalMidpoint);

		if (rect.left < verticalMidpoint && rect.left + rect.width < verticalMidpoint)
		{
			if (topQuadrant)
			{
				index = 1;
			}
			else if (bottomQuadrant)
			{
				index = 2;
			}
		}
		else if (rect.left > verticalMidpoint)
		{
			if (topQuadrant)
			{
				index = 0;
			}
			else if (bottomQuadrant)
			{
				index = 3;
			}
		}

		return index;
	}
}