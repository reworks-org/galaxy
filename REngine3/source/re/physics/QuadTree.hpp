//
//  QuadTree.hpp
//  REngine3
//
//  Created by reworks on 28/08/2017.
//  Code ported from:
//  https://gamedevelopment.tutsplus.com/tutorials/quick-tip-use-quadtrees-to-detect-likely-collisions-in-2d-space--gamedev-374

#ifndef RENGINE3_QUADTREE_HPP_
#define RENGINE3_QUADTREE_HPP_

#include <array>
#include <vector>

#include <SFML/Graphics/Rect.hpp>

#include "re/entity/Entity.hpp"

namespace re
{
	class QuadTree
	{
	public:
		/*
		* Constructor
		*/
		QuadTree(int level, sf::Rect<int>& bounds, int maxLevels = 5, int maxObjects = 10);

		/*
		* Clears the quadtree
		*/
		void clear();

		/*
		* Insert the object into the quadtree. If the node
		* exceeds the capacity, it will split and add all
		* objects to their corresponding nodes.
		*/
		void insert(Entity* entity);

		/*
		* Return all objects that could collide with the given object
		*/
		std::vector<Entity*>& retrieve(std::vector<Entity*>& returnObjects, Entity* entity);

	private:
		/*
		* Splits the node into 4 subnodes
		*/
		void split();

		/*
		* Determine which node the object belongs to. -1 means
		* object cannot completely fit within a child node and is part
		* of the parent node
		*/
		int getIndex(sf::Rect<int>& rect);

	private:
		int m_level;
		int m_maxLevels;
		int m_maxObjects;
		std::vector<Entity*> m_objects;
		sf::Rect<int> m_bounds;
		std::array<QuadTree*, 4> m_nodes;
	};
}

#endif