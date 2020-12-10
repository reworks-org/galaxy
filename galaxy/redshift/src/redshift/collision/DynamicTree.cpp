///
/// DynamicTree.cpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#include <pulsar/Log.hpp>

#include "DynamicTree.hpp"

#define DEFAULT_SIZE 10

///
/// Core namespace.
///
namespace rs
{
	DynamicTree::Node::Node()
	    : m_parent_node {s_NULL_NODE}, m_left_node {s_NULL_NODE}, m_right_node {s_NULL_NODE}, m_next_node {s_NULL_NODE}
	{
	}

	const bool DynamicTree::Node::is_leaf() const
	{
		return m_left_node == s_NULL_NODE;
	}

	DynamicTree::DynamicTree()
	    : m_root_node {Node::s_NULL_NODE},
	      m_allocated_nodes {0},
	      m_next_free_node {0},
	      m_node_capacity {DEFAULT_SIZE},
	      m_growth_size {1}
	{
		m_nodes.resize(DEFAULT_SIZE);
		for (auto index = 0; index < DEFAULT_SIZE; index++)
		{
			m_nodes[index].m_next_node = index + 1;
		}

		m_nodes[DEFAULT_SIZE - 1].m_next_node = Node::s_NULL_NODE;
	}

	DynamicTree::DynamicTree(const int base_size)
	    : m_root_node {Node::s_NULL_NODE},
	      m_allocated_nodes {0},
	      m_next_free_node {0},
	      m_node_capacity {base_size},
	      m_growth_size {1}
	{
		m_nodes.resize(base_size);
		for (auto index = 0; index < base_size; index++)
		{
			m_nodes[index].m_next_node = index + 1;
		}

		m_nodes[base_size - 1].m_next_node = Node::s_NULL_NODE;
	}

	DynamicTree::~DynamicTree()
	{
		while (!m_node_stack.empty())
		{
			m_node_stack.pop();
		}

		m_hits.clear();
		m_nodes.clear();
		m_object_node_index.clear();
		m_merged.clear();
	}

	void DynamicTree::insert(std::weak_ptr<Collidable> object)
	{
		if (auto ptr = object.lock())
		{
			const int node         = allocate_node();
			m_nodes[node].m_aabb   = &ptr->get_aabb();
			m_nodes[node].m_object = object;

			insert_leaf(node);
			m_object_node_index[object] = node;
		}
		else
		{
			PL_LOG(PL_FATAL, "Attempted to access deleted memory. Invalid Weak Pointer.");
		}
	}

	void DynamicTree::remove(std::weak_ptr<Collidable> object)
	{
		const int node = m_object_node_index[object];

		remove_leaf(node);
		deallocate_node(node);

		m_object_node_index.erase(object);
	}

	void DynamicTree::update(std::weak_ptr<Collidable> object)
	{
		if (auto ptr = object.lock())
		{
			const int node = m_object_node_index[object];
			update_leaf(node, ptr->get_aabb());
		}
		else
		{
			PL_LOG(PL_FATAL, "Attempted to access deleted memory. Invalid Weak Pointer.");
		}
	}

	auto DynamicTree::query(std::weak_ptr<Collidable> object) -> std::forward_list<std::shared_ptr<Collidable>>&
	{
		// Clear out old data.
		m_hits.clear();
		while (!m_node_stack.empty())
		{
			m_node_stack.pop();
		}

		if (auto ptr = object.lock())
		{
			const AABB& aabb_to_test = ptr->get_aabb();
			m_node_stack.push(m_root_node);

			while (!m_node_stack.empty())
			{
				const int node = m_node_stack.top();
				m_node_stack.pop();

				if (node != Node::s_NULL_NODE)
				{
					const Node& node_obj = m_nodes[node];
					if (node_obj.m_aabb->overlaps(aabb_to_test))
					{
						if (auto node_ptr = node_obj.m_object.lock())
						{
							if (node_obj.is_leaf() && node_ptr != ptr)
							{
								m_hits.emplace_front(node_ptr);
							}
							else
							{
								m_node_stack.push(node_obj.m_left_node);
								m_node_stack.push(node_obj.m_right_node);
							}
						}
						else
						{
							PL_LOG(PL_FATAL, "Attempted to access deleted memory. Invalid Weak Pointer.");
						}
					}
				}
			}
		}
		else
		{
			PL_LOG(PL_FATAL, "Attempted to access deleted memory. Invalid Weak Pointer.");
		}

		return m_hits;
	}

	int DynamicTree::allocate_node()
	{
		if (m_next_free_node == Node::s_NULL_NODE)
		{
			if (m_allocated_nodes != m_node_capacity)
			{
				PL_LOG(PL_FATAL, "m_allocated_nodes != m_node_capacity");
			}
			else
			{
				m_node_capacity += m_growth_size;
				m_nodes.resize(m_node_capacity);

				for (auto index = m_allocated_nodes; index < m_node_capacity; index++)
				{
					m_nodes[index].m_next_node = index + 1;
				}

				m_nodes[m_node_capacity - 1].m_next_node = Node::s_NULL_NODE;

				m_next_free_node = m_allocated_nodes;
			}
		}

		int index                    = m_next_free_node;
		m_nodes[index].m_parent_node = Node::s_NULL_NODE;
		m_nodes[index].m_left_node   = Node::s_NULL_NODE;
		m_nodes[index].m_right_node  = Node::s_NULL_NODE;
		m_next_free_node             = m_nodes[index].m_next_node;
		m_allocated_nodes++;

		return index;
	}

	void DynamicTree::deallocate_node(const int node)
	{
		m_nodes[node].m_next_node = m_next_free_node;
		m_next_free_node          = node;
		m_allocated_nodes--;
	}

	void DynamicTree::insert_leaf(const int leaf)
	{
		if ((m_nodes[leaf].m_parent_node == Node::s_NULL_NODE) ||
		    (m_nodes[leaf].m_left_node == Node::s_NULL_NODE) ||
		    (m_nodes[leaf].m_right_node == Node::s_NULL_NODE))
		{
			PL_LOG(PL_FATAL, "Attempted to insert invalid leaf.");
		}
		else
		{
			if (m_root_node == Node::s_NULL_NODE)
			{
				m_root_node = leaf;
			}
			else
			{
				int tree_node  = m_root_node;
				Node& leaf_obj = m_nodes[leaf];
				while (!m_nodes[tree_node].is_leaf())
				{
					const Node& tree_obj  = m_nodes[tree_node];
					const int left_node   = tree_obj.m_left_node;
					const int right_node  = tree_obj.m_right_node;
					const Node& left_obj  = m_nodes[left_node];
					const Node& right_obj = m_nodes[right_node];

					AABB merged = tree_obj.m_aabb->merge(*leaf_obj.m_aabb);

					const float parent_node_cost = 2.0f * merged.surface_area();
					const float push_down_cost   = 2.0f * (merged.surface_area() - tree_obj.m_aabb->surface_area());

					float left_cost  = 0.0f;
					float right_cost = 0.0f;
					if (left_obj.is_leaf())
					{
						left_cost = leaf_obj.m_aabb->merge(*left_obj.m_aabb).surface_area() + push_down_cost;
					}
					else
					{
						AABB left_leaf_merge = leaf_obj.m_aabb->merge(*left_obj.m_aabb);
						left_cost            = (left_leaf_merge.surface_area() - left_obj.m_aabb->surface_area()) + push_down_cost;
					}

					if (right_obj.is_leaf())
					{
						right_cost = leaf_obj.m_aabb->merge(*right_obj.m_aabb).surface_area() + push_down_cost;
					}
					else
					{
						AABB right_leaf_merge = leaf_obj.m_aabb->merge(*right_obj.m_aabb);
						right_cost            = (right_leaf_merge.surface_area() - right_obj.m_aabb->surface_area()) + push_down_cost;
					}

					if (parent_node_cost < left_cost && parent_node_cost < right_cost)
					{
						break;
					}
					else
					{
						if (left_cost < right_cost)
						{
							tree_node = left_node;
						}
						else
						{
							tree_node = right_node;
						}
					}
				}

				Node& leaf_sibling_obj    = m_nodes[tree_node];
				const int old_parent_node = leaf_sibling_obj.m_parent_node;
				const int new_parent_node = allocate_node();

				Node& new_parent_obj           = m_nodes[new_parent_node];
				new_parent_obj.m_parent_node   = old_parent_node;
				new_parent_obj.m_aabb          = &m_merged.emplace_back(leaf_sibling_obj.m_aabb->merge(*leaf_sibling_obj.m_aabb));
				new_parent_obj.m_left_node     = tree_node;
				new_parent_obj.m_right_node    = leaf;
				leaf_obj.m_parent_node         = new_parent_node;
				leaf_sibling_obj.m_parent_node = new_parent_node;

				if (old_parent_node == Node::s_NULL_NODE)
				{
					m_root_node = new_parent_node;
				}
				else
				{
					Node& old_parent_obj = m_nodes[old_parent_node];
					if (old_parent_obj.m_left_node == tree_node)
					{
						old_parent_obj.m_left_node = new_parent_node;
					}
					else
					{
						old_parent_obj.m_right_node = new_parent_node;
					}
				}

				tree_node = leaf_obj.m_parent_node;
				fix_upwards_tree(tree_node);
			}
		}
	}

	void DynamicTree::remove_leaf(const int leaf)
	{
		if (leaf == m_root_node)
		{
			m_root_node = Node::s_NULL_NODE;
		}
		else
		{
			Node& leaf_obj              = m_nodes[leaf];
			const int parent_node       = leaf_obj.m_parent_node;
			const Node& parent_obj      = m_nodes[parent_node];
			const int grand_parent_node = parent_obj.m_parent_node;
			const int sibling_node      = parent_obj.m_left_node == leaf ? parent_obj.m_right_node : parent_obj.m_left_node;
			if (sibling_node != Node::s_NULL_NODE)
			{
				Node& sibling_obj = m_nodes[sibling_node];
				if (grand_parent_node != Node::s_NULL_NODE)
				{
					Node& grand_parent_obj = m_nodes[grand_parent_node];
					if (grand_parent_obj.m_left_node == parent_node)
					{
						grand_parent_obj.m_left_node = sibling_node;
					}
					else
					{
						grand_parent_obj.m_right_node = sibling_node;
					}

					sibling_obj.m_parent_node = grand_parent_node;
					deallocate_node(parent_node);
					fix_upwards_tree(grand_parent_node);
				}
				else
				{
					m_root_node               = sibling_node;
					sibling_obj.m_parent_node = Node::s_NULL_NODE;
					deallocate_node(parent_node);
				}

				leaf_obj.m_parent_node = Node::s_NULL_NODE;
			}
		}
	}

	void DynamicTree::update_leaf(const int leaf, AABB& aabb)
	{
		Node& leaf_obj = m_nodes[leaf];

		if (!leaf_obj.m_aabb->contains(aabb))
		{
			remove_leaf(leaf);
			leaf_obj.m_aabb = &aabb;
			insert_leaf(leaf);
		}
	}

	void DynamicTree::fix_upwards_tree(int tree_node)
	{
		while (tree_node != Node::s_NULL_NODE)
		{
			Node& tree_obj = m_nodes[tree_node];
			if (tree_obj.m_left_node != Node::s_NULL_NODE && tree_obj.m_right_node != Node::s_NULL_NODE)
			{
				const Node& left_obj  = m_nodes[tree_obj.m_left_node];
				const Node& right_obj = m_nodes[tree_obj.m_right_node];

				tree_obj.m_aabb = &m_merged.emplace_back(left_obj.m_aabb->merge(*right_obj.m_aabb));
				tree_node       = tree_obj.m_parent_node;
			}
		}
	}
} //namespace rs