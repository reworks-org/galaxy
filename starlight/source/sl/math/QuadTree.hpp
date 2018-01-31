///
/// QuadTree.hpp
/// starlight
///
/// Written by reworks on 28/08/2017.
/// Code ported from:
/// https://gamedevelopment.tutsplus.com/tutorials/quick-tip-use-quadtrees-to-detect-likely-collisions-in-2d-space--gamedev-374

#ifndef RENGINE3_QUADTREE_HPP_
#define RENGINE3_QUADTREE_HPP_

#include <array>
#include <vector>

#include "entt/entt.hpp"
#include "sl/math/Rect.hpp"

namespace sl
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
		QuadTree(int level, Rect<float, int>& bounds, int maxLevels = 5, int maxObjects = 10);

		///
		/// Constructor. CONST BOUNDS
		///
		/// \param level The level of the quadtree inside the tree. 0 for surface.
		/// \param bounds CONST Bounds of the quadtree, usually the level.
		/// \param maxLevels Maximum number of levels inside the quadtree.
		/// \param maxObjects Maximum number of objects inside a node.
		///
		QuadTree(int level, const Rect<float, int>& bounds, int maxLevels = 5, int maxObjects = 10);

		///
		/// Destructor.
		///
		~QuadTree();

		///
		/// Clears the quadtree of all data.
		///
		void clear();

		///
		/// Update the quadtree bounds.
		///
		/// \param newBounds a Rect of type <float, int>.
		///
		void updateBounds(const Rect<float, int>& newBounds);

		///
		/// \brief Insert the object into the quadtree.
		///
		/// If the node exceeds the capacity, it will split and add all objects to their corresponding nodes.
		///
		/// \param entity The entity to insert.
		///
		void insert(entt::Entity e);

		///
		/// Return all objects that could collide with the given entity.
		///
		/// \param returnObjects A vector containing the entitys that could collide with the entity.
		/// \param entity Entity to check for collisions with.
		///
		void retrieve(std::vector<entt::Entity>& returnObjects, entt::Entity e);

		///
		/// Return all objects that could collide with the given rect.
		///
		/// \param returnObjects A vector containing the entitys that could collide with the entity.
		/// \param rect Rect to check for collisions with.
		///
		void retrieve(std::vector<entt::Entity>& returnObjects, Rect<float, int>& rect);

		///
		/// Return all objects that could collide with the given rect. CONST FUNCTION
		///
		/// \param returnObjects A vector containing the entitys that could collide with the entity.
		/// \param rect CONST Rect to check for collisions with.
		///
		void retrieve(std::vector<entt::Entity>& returnObjects, const Rect<float, int>& rect);

	private:
		///
		/// Default constructor.
		///
		QuadTree() = delete;

		///
		/// Splits the node into 4 subnodes.
		///
		void split();

		///
		/// \brief Determine which node the object belongs to.
		///
		/// -1 means object cannot completely fit within a child node and is part of the parent node.
		///
		int getIndex(Rect<float, int>& rect);

		///
		/// \brief Determine which node the object belongs to. CONST FUNCTION
		///
		/// -1 means object cannot completely fit within a child node and is part of the parent node.
		///
		int getIndex(const Rect<float, int>& rect);

	private:
		int m_level;
		int m_maxLevels;
		int m_maxObjects;
		std::vector<entt::Entity> m_objects;
		Rect<float, int> m_bounds;
		std::array<QuadTree*, 4> m_nodes;
	};
}

#endif