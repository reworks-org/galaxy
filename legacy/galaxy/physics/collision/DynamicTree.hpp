///
/// DynamicTree.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_PHYSICS_COLLISION_DYNAMICTREE_HPP_
#define GALAXY_PHYSICS_COLLISION_DYNAMICTREE_HPP_

#include <forward_list>
#include <memory>
#include <stack>

#include <robin_hood.h>

#include "galaxy/physics/collision/Collidable.hpp"

namespace galaxy
{
	namespace physics
	{
		///
		/// Used for broad phase collision detection.
		/// See:
		/// https://www.azurefromthetrenches.com/introductory-guide-to-aabb-tree-collision-detection/
		///
		class DynamicTree final
		{
		public:
			///
			/// Constructor.
			///
			DynamicTree();

			///
			/// Argument constructor.
			///
			/// \param base_size Starting size of the dynamic tree.
			///
			DynamicTree(const int base_size);

			///
			/// Destructor.
			///
			~DynamicTree();

			///
			/// Insert an object.
			///
			/// \param object Object to insert.
			///
			void insert(std::shared_ptr<Collidable> object);

			///
			/// Remove an object.
			///
			/// \param object Object to remove.
			///
			void remove(std::shared_ptr<Collidable> object);

			///
			/// Update an object.
			///
			/// \param object Object to update.
			///
			void update(std::shared_ptr<Collidable> object);

			///
			/// Query an object for possible collisions.
			///
			/// \param object Object to query.
			///
			/// \return List of possible objects that could have been collided with.
			///
			[[nodiscard]] auto query(std::shared_ptr<Collidable> object) -> std::forward_list<std::shared_ptr<Collidable>>;

		private:
			///
			/// A node in the tree.
			///
			struct Node
			{
				///
				/// Represents the value of an invalid node.
				///
				static const constexpr int s_NULL_NODE = -1;

				///
				/// Constructor.
				///
				Node() noexcept;

				///
				/// Destructor.
				///
				~Node() noexcept = default;

				///
				/// Is this node a leaf?
				///
				[[nodiscard]] const bool is_leaf() const noexcept;

				///
				/// Pointer to AABB to be used by this node.
				///
				AABB* m_aabb;

				///
				/// Pointer to the object this node represents.
				///
				std::shared_ptr<Collidable> m_object;

				///
				/// Parent node id.
				///
				int m_parent_node;

				///
				/// Left node id.
				///
				int m_left_node;

				///
				/// Right node id.
				///
				int m_right_node;

				///
				/// Next node id.
				///
				int m_next_node;
			};

			///
			/// Add a node.
			///
			int allocate_node();

			///
			/// Remove a node.
			///
			void deallocate_node(const int node);

			///
			/// Insert a leaf node.
			///
			void insert_leaf(const int leaf);

			///
			/// Remove a leaf node.
			///
			void remove_leaf(const int leaf);

			///
			/// You need to keep AABB in memory.
			///
			void update_leaf(const int leaf, AABB& aabb);

			///
			/// Update tree after making changes.
			///
			void fix_upwards_tree(int tree_node);

			///
			/// Copy constructor.
			///
			DynamicTree(const DynamicTree&) = delete;

			///
			/// Move constructor.
			///
			DynamicTree(DynamicTree&&) = delete;

			///
			/// Copy assignment operator.
			///
			DynamicTree& operator=(const DynamicTree&) = delete;

			///
			/// Move assignment operator.
			///
			DynamicTree& operator=(DynamicTree&&) = delete;

		private:
			///
			/// Map of objects and their nodes.
			///
			robin_hood::unordered_map<std::shared_ptr<Collidable>, int> m_object_node_index;

			///
			/// List of nodes.
			///
			std::vector<Node> m_nodes;

			///
			/// List of merged AABB objects.
			///
			std::vector<AABB> m_merged;

			///
			/// Root node id.
			///
			int m_root_node;

			///
			/// Number of created nodes.
			///
			int m_allocated_nodes;

			///
			/// Next free node id.
			///
			int m_next_free_node;

			///
			/// Total capacity for nodes.
			///
			int m_node_capacity;

			///
			/// How much the tree should grow by for each node added.
			///
			int m_growth_size;

			///
			/// Tree stack.
			///
			std::stack<int> m_node_stack;
		};
	} // namespace physics
} // namespace galaxy

#endif