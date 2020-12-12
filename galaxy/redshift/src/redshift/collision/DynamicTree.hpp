///
/// DynamicTree.hpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#ifndef REDSHIFT_DYNAMICTREE_HPP_
#define REDSHIFT_DYNAMICTREE_HPP_

#include <forward_list>
#include <memory>
#include <stack>

#include "robin_hood.h"

#include "redshift/collision/Collidable.hpp"

///
/// Core namespace.
///
namespace rs
{
	///
	/// Used for broad phase collision detection.
	/// See:
	/// https://www.azurefromthetrenches.com/introductory-guide-to-aabb-tree-collision-detection/
	///
	class DynamicTree final
	{
	public:
		DynamicTree();
		DynamicTree(const int base_size);
		~DynamicTree();

		void insert(std::weak_ptr<Collidable> object);
		void remove(std::weak_ptr<Collidable> object);
		void update(std::weak_ptr<Collidable> object);

		[[nodiscard]] auto query(std::weak_ptr<Collidable> object) -> std::forward_list<std::shared_ptr<Collidable>>&;

	private:
		struct Node
		{
			static const constexpr int s_NULL_NODE = -1;

			Node();
			~Node() = default;

			[[nodiscard]] const bool is_leaf() const;

			AABB* m_aabb;
			std::weak_ptr<Collidable> m_object;

			int m_parent_node;
			int m_left_node;
			int m_right_node;
			int m_next_node;
		};

		int allocate_node();
		void deallocate_node(const int node);
		void insert_leaf(const int leaf);
		void remove_leaf(const int leaf);

		///
		/// You need to keep AABB in memory.
		///
		void update_leaf(const int leaf, AABB& aabb);

		void fix_upwards_tree(int tree_node);

		robin_hood::unordered_map<std::shared_ptr<Collidable>, int> m_object_node_index;
		std::vector<Node> m_nodes;
		std::vector<AABB> m_merged;
		int m_root_node;
		int m_allocated_nodes;
		int m_next_free_node;
		int m_node_capacity;
		int m_growth_size;

		std::forward_list<std::shared_ptr<Collidable>> m_hits;
		std::stack<int> m_node_stack;
	};
} //namespace rs

#endif