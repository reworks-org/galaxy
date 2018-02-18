///
/// QuadTree.cpp
/// starlight
///
/// Written by reworks on 28/08/2017.
/// Code ported from:
/// https://gamedevelopment.tutsplus.com/tutorials/quick-tip-use-quadtrees-to-detect-likely-collisions-in-2d-space--gamedev-374

#include "sl/core/World.hpp"
#include "sl/core/ServiceLocator.hpp"
#include "sl/components/TransformComponent.hpp"

#include "QuadTree.hpp"

namespace sl
{
	QuadTree::QuadTree(int level, Rect<float, int>& bounds, int maxLevels, int maxObjects)
	:m_level(level), m_bounds(bounds), m_maxLevels(maxLevels), m_maxObjects(maxObjects)
	{
		for (auto& elem : m_nodes)
		{
			elem = nullptr;
		}
	}

	QuadTree::QuadTree(int level, const Rect<float, int>& bounds, int maxLevels, int maxObjects)
		:m_level(level), m_bounds(bounds), m_maxLevels(maxLevels), m_maxObjects(maxObjects)
	{
		for (auto& elem : m_nodes)
		{
			elem = nullptr;
		}
	}

	QuadTree::~QuadTree()
	{
		clear();
	}

	void QuadTree::clear()
	{
		m_objects.clear();

		for (int i = 0; i <  m_nodes.size(); i++)
		{
			if (m_nodes[i] != nullptr)
			{
				m_nodes[i]->clear();
				delete m_nodes[i];
				m_nodes[i] = nullptr;
			}
		}
	}

	void QuadTree::updateBounds(const Rect<float, int>& newBounds)
	{
		m_bounds = newBounds;
	}

	void QuadTree::insert(entt::Entity e)
	{
		auto tc_rect = Locator::m_world->m_registry.get<TransformComponent>(e).m_rect;

		if (m_nodes[0] != nullptr)
		{
			int index = getIndex(tc_rect);

			if (index != -1) {
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

			int i = 0;
			while (i < m_objects.size())
			{
				int index = getIndex(Locator::m_world->m_registry.get<TransformComponent>(m_objects[i]).m_rect);
				if (index != -1)
				{
					m_nodes[index]->insert(m_objects[i]);
					m_objects.erase(m_objects.begin() + i);
				}
				else 
				{
					i++;
				}
			}
		}
	}

	void QuadTree::retrieve(std::vector<entt::Entity>& returnObjects, entt::Entity e)
	{
		auto tc_rect = Locator::m_world->m_registry.get<TransformComponent>(e).m_rect;
		
		int index = getIndex(tc_rect);
		if (index != -1 && m_nodes[0] != nullptr)
		{
			m_nodes[index]->retrieve(returnObjects, tc_rect);
		}

		returnObjects.insert(returnObjects.end(), m_objects.begin(), m_objects.end());
	}

	void QuadTree::retrieve(std::vector<entt::Entity>& returnObjects, Rect<float, int>& rect)
	{
		int index = getIndex(rect);
		if (index != -1 && m_nodes[0] != nullptr)
		{
			m_nodes[index]->retrieve(returnObjects, rect);
		}

		returnObjects.insert(returnObjects.end(), m_objects.begin(), m_objects.end());
	}

	void QuadTree::retrieve(std::vector<entt::Entity>& returnObjects, const Rect<float, int>& rect)
	{
		int index = getIndex(rect);
		if (index != -1 && m_nodes[0] != nullptr)
		{
			m_nodes[index]->retrieve(returnObjects, rect);
		}

		returnObjects.insert(returnObjects.end(), m_objects.begin(), m_objects.end());
	}

	void QuadTree::split()
	{
		int subWidth = (m_bounds.m_width / 2);
		int subHeight = (m_bounds.m_height / 2);

		m_nodes[0] = new QuadTree(m_level + 1, Rect<float, int>(m_bounds.m_x + subWidth, m_bounds.m_y, subWidth, subHeight), m_maxLevels, m_maxObjects);
		m_nodes[1] = new QuadTree(m_level + 1, Rect<float, int>(m_bounds.m_x, m_bounds.m_y, subWidth, subHeight), m_maxLevels, m_maxObjects);
		m_nodes[2] = new QuadTree(m_level + 1, Rect<float, int>(m_bounds.m_x, m_bounds.m_y + subHeight, subWidth, subHeight), m_maxLevels, m_maxObjects);
		m_nodes[3] = new QuadTree(m_level + 1, Rect<float, int>(m_bounds.m_x + subWidth, m_bounds.m_y + subHeight, subWidth, subHeight), m_maxLevels, m_maxObjects);
	}

	int QuadTree::getIndex(Rect<float, int>& rect)
	{
		int index = -1;
		float verticalMidpoint = m_bounds.m_x + (m_bounds.m_width / 2);
		float horizontalMidpoint = m_bounds.m_y + (m_bounds.m_height / 2);

		// Object can completely fit within the top quadrants
		bool topQuadrant = (rect.m_y < horizontalMidpoint && rect.m_y + rect.m_height < horizontalMidpoint);

		// Object can completely fit within the bottom quadrants
		bool bottomQuadrant = (rect.m_y > horizontalMidpoint);

		// Object can completely fit within the left quadrants
		if (rect.m_x < verticalMidpoint && rect.m_x + rect.m_width < verticalMidpoint)
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
		// Object can completely fit within the right quadrants
		else if (rect.m_x > verticalMidpoint)
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

	int QuadTree::getIndex(const Rect<float, int>& rect)
	{
		int index = -1;
		float verticalMidpoint = m_bounds.m_x + (m_bounds.m_width / 2);
		float horizontalMidpoint = m_bounds.m_y + (m_bounds.m_height / 2);

		// Object can completely fit within the top quadrants
		bool topQuadrant = (rect.m_y < horizontalMidpoint && rect.m_y + rect.m_height < horizontalMidpoint);

		// Object can completely fit within the bottom quadrants
		bool bottomQuadrant = (rect.m_y > horizontalMidpoint);

		// Object can completely fit within the left quadrants
		if (rect.m_x < verticalMidpoint && rect.m_x + rect.m_width < verticalMidpoint)
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
		// Object can completely fit within the right quadrants
		else if (rect.m_x > verticalMidpoint)
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