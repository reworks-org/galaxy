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

#include "re/math/Rect.hpp"
#include "re/core/Entity.hpp"

namespace re
{
	class QuadTree
	{
	public:
		///
		/// Constructor.
		///
		/// \param level The level of the quadtree inside the tree. 0 for surface.
		/// \param bounds Bounds of the quadtree, usually the level.
		/// \param maxLevels Maximum number of levels inside the quadtree.
		/// \param maxObjects Maximum number of objects inside a node.
		///
		QuadTree(int level, Rect<int>& bounds, int maxLevels = 5, int maxObjects = 10);

		///
		/// Clears the quadtree of all data.
		///
		void clear();

		///
		/// \brief Insert the object into the quadtree.
		///
		/// If the node exceeds the capacity, it will split and add all objects to their corresponding nodes.
		///
		/// \param entity The entity to insert.
		///
		void insert(Entity* entity);

		///
		/// Return all objects that could collide with the given object.
		///
		/// \param returnObjects A vector containing the entitys that could collide with the entity.
		/// \param entity Entity to check for collisions with.
		///
		void retrieve(std::vector<Entity*>& returnObjects, Entity* entity);

	private:
		///
		/// Splits the node into 4 subnodes.
		///
		void split();

		///
		/// \brief Determine which node the object belongs to.
		///
		/// -1 means object cannot completely fit within a child node and is part of the parent node.
		///
		int getIndex(Rect<int>& rect);

	private:
		size_t m_level;
		size_t m_maxLevels;
		size_t m_maxObjects;
		std::vector<Entity*> m_objects;
		Rect<int> m_bounds;
		std::array<QuadTree*, 4> m_nodes;
	};
}

#endif