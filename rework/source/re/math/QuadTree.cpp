//
//  QuadTree.cpp
//  REngine3
//
//  Created by reworks on 28/08/2017.
//  Code ported from:
//  https://gamedevelopment.tutsplus.com/tutorials/quick-tip-use-quadtrees-to-detect-likely-collisions-in-2d-space--gamedev-374

#include "re/components/TransformComponent.hpp"

#include "QuadTree.hpp"

namespace re
{
	QuadTree::QuadTree(size_t level, Rect<float, int>& bounds, size_t maxLevels, size_t maxObjects)
	:m_level(level), m_bounds(bounds), m_maxLevels(maxLevels), m_maxObjects(maxObjects)
	{
		for (auto& elem : m_nodes)
		{
			elem = nullptr;
		}
	}

	void QuadTree::clear()
	{
		m_objects.clear();

		for (size_t i = 0; i < m_nodes.size(); ++i)
		{
			if (m_nodes[i] != nullptr)
			{
				m_nodes[i]->clear();
				delete m_nodes[i];
				m_nodes[i] = nullptr;
			}
		}
	}

	void QuadTree::insert(entityx::Entity& e)
	{
		auto& tc_rect = e.component<TransformComponent>()->m_rect;

		Rect<float, int> rect(tc_rect.x, tc_rect.y, tc_rect.width, tc_rect.height);

		if (m_nodes[0] != nullptr)
		{
			int index = getIndex(rect);
			if (index != -1)
			{
				m_nodes[index]->insert(e);
				
				return;
			}
		}

		m_objects.push_back(e);

		if (m_objects.size() > m_maxObjects && m_level < m_maxLevels)
		{
			if (m_nodes[0] == nullptr)
			{
				split();
			}

			size_t i = 0;
			while (i < m_objects.size())
			{
				auto& tc_rect_2 = m_objects[i].component<TransformComponent>()->m_rect;

				Rect<float, int> rect_(tc_rect_2.x, tc_rect_2.y, tc_rect_2.width, tc_rect_2.height);

				int index = getIndex(rect_);
				if (index != -1)
				{
					m_nodes[index]->insert(m_objects[i]);
					m_objects.erase(m_objects.begin() + i);
				}
				else
				{
					++i;
				}
			}
		}
	}

	void QuadTree::retrieve(std::vector<entityx::Entity>& returnObjects, entityx::Entity& e)
	{
		auto& tc_rect = e.component<TransformComponent>()->m_rect;

		Rect<float, int> rect(tc_rect.x, tc_rect.y, tc_rect.width, tc_rect.height);

		int index = getIndex(rect);
		if (index != -1 && m_nodes[0] != nullptr)
		{
			m_nodes[index]->retrieve(returnObjects, e);
		}

		returnObjects.insert(returnObjects.begin(), m_objects.begin(), m_objects.end());
	}

	void QuadTree::retrieve(std::vector<entityx::Entity>& returnObjects, Rect<float, int>& rect)
	{
		Rect<float, int> local_rect(rect.x, rect.y, rect.width, rect.height);

		int index = getIndex(local_rect);
		if (index != -1 && m_nodes[0] != nullptr)
		{
			m_nodes[index]->retrieve(returnObjects, rect);
		}

		returnObjects.insert(returnObjects.begin(), m_objects.begin(), m_objects.end());
	}

	void QuadTree::split()
	{
		int subWidth = (m_bounds.width / 2);
		int subHeight = (m_bounds.height / 2);
		float x = m_bounds.x;
		float y = m_bounds.y;
		
		m_nodes[0] = new QuadTree(m_level + (size_t)1, Rect<float, int>(x + subWidth, y, subWidth, subHeight), m_maxLevels, m_maxObjects);
		m_nodes[1] = new QuadTree(m_level + (size_t)1, Rect<float, int>(x, y, subWidth, subHeight), m_maxLevels, m_maxObjects);
		m_nodes[2] = new QuadTree(m_level + (size_t)1, Rect<float, int>(x, y + subHeight, subWidth, subHeight), m_maxLevels, m_maxObjects);
		m_nodes[3] = new QuadTree(m_level + (size_t)1, Rect<float, int>(x + subWidth, y + subHeight, subWidth, subHeight), m_maxLevels, m_maxObjects);
	}

	int QuadTree::getIndex(Rect<float, int>& rect)
	{
		int index = -1;
		double verticalMidpoint = m_bounds.x + (m_bounds.width / 2);
		double horizontalMidpoint = m_bounds.y + (m_bounds.height / 2);

		bool topQuadrant = (rect.y < horizontalMidpoint && rect.y + rect.height < horizontalMidpoint);
		bool bottomQuadrant = (rect.y > horizontalMidpoint);

		if (rect.x < verticalMidpoint && rect.x + rect.width < verticalMidpoint)
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
		else if (rect.x > verticalMidpoint)
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