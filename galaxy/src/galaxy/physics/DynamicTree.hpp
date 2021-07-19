/*
 * MIT License
 *
 * Copyright (c) 2019-2020 Albin Johansson: adapted and improved source code
 * from the AABBCC library.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * This codebase was mainly based on the AABBCC library, written by Lester
 * Hedges, which uses the Zlib license: https://github.com/lohedges/aabbcc.
 * Furthermore, the AABB tree implementation in the Simple Voxel Engine project
 * also influenced this library, which uses the MIT license:
 * https://github.com/JamesRandall/SimpleVoxelEngine.
 */

/*
	GALAXY ENGINE EDIT:
	THIS CODE FILE HAS BEEN RENAMED FROM abby.hpp TO DynamicTree.hpp.
	THIS CODEBASE HAS ALSO BEEN EXTENSIVELY MODIFIED FOR USE IN GALAXY.
*/

#ifndef GALAXY_PHYSICS_DYNAMICTREE_HPP_
#define GALAXY_PHYSICS_DYNAMICTREE_HPP_

#include <algorithm>
#include <array>
#include <memory_resource>
#include <stack>
#include <vector>

#include <robin_hood.h>

#include "galaxy/error/Log.hpp"
#include "galaxy/math/AABB.hpp"

namespace galaxy
{
	namespace physics
	{
		using maybe_index = std::optional<std::size_t>;

		///
		/// \brief Represents a node in an AABB tree.
		///
		/// Contains an AABBand the entity associated with the AABB,
		/// along with tree information.
		///
		template<typename Key, typename T>
		struct Node final
		{
			using key_type = Key;

			std::optional<key_type> id;
			math::AABB aabb;

			maybe_index parent;
			maybe_index left;
			maybe_index right;
			maybe_index next;
			int height = -1;

			[[nodiscard]] inline const bool is_leaf() const noexcept
			{
				return left == std::nullopt;
			}
		};

		///
		/// Represents a tree of AABBs used for efficient collision detection.
		///
		template<typename Key>
		class DynamicTree final
		{
			template<typename U>
			using pmr_stack = std::stack<U, std::pmr::deque<U>>;

		public:
			using value_type = float;
			using key_type   = Key;
			using node_type  = Node<key_type, value_type>;
			using size_type  = std::size_t;
			using index_type = size_type;

			///
			/// Creates an AABB tree.
			///
			/// \param capacity The initial node capacity of the tree.
			///
			inline explicit DynamicTree(const size_type capacity = 16)
				: m_node_capacity {capacity}
			{
				resize_to_match_node_capacity(0);
			}

			///
			/// Inserts an AABB in the tree.
			///
			/// \param key The ID that will be associated with the box.
			/// \param lower_bound The lower - bound position of the AABB(i.e.the position).
			///	\param upper_bound The upper - bound position of the AABB.
			///
			inline void insert(const key_type& key, const glm::vec2& lower_bound, const glm::vec2& upper_bound)
			{
				if (!m_index_map.count(key))
				{
					// Allocate a new node for the particle
					const auto node_index = allocate_node();
					auto& node            = m_nodes.at(node_index);
					node.id               = key;
					node.aabb             = {lower_bound, upper_bound};
					node.aabb.fatten(m_skin_thickness);
					node.height = 0;

					insert_leaf(node_index);
					m_index_map.emplace(key, node_index);
				}
				else
				{
					GALAXY_LOG(GALAXY_WARNING, "Cannot insert duplicate key.");
				}
			}

			///
			/// Removes the AABB associated with the specified ID.
			///
			/// This function has no effect if there is no AABB associated with the specified ID.
			///
			/// \param key The ID associated with the AABB that will be removed.
			///
			inline void erase(const key_type& key)
			{
				if (const auto it = m_index_map.find(key); it != m_index_map.end())
				{
					const auto node = it->second; // Extract the node index.

					m_index_map.erase(it);

					remove_leaf(node);
					free_node(node);
				}
			}

			///
			/// Clears the tree of all entries.
			///
			inline void clear()
			{
				// Iterator pointing to the start of the particle map.
				auto it = m_index_map.begin();

				// Iterate over the map.
				while (it != m_index_map.end())
				{
					// Extract the node index.
					const auto node_index = it->second;

					remove_leaf(node_index);
					free_node(node_index);

					++it;
				}

				// Clear the particle map.
				m_index_map.clear();
			}

			///
			/// \brief Updates the AABB associated with the specified ID.
			///
			/// This function has no effect if there is no AABB associated with the specified ID.
			///
			/// \param key The ID associated with the AABB that will be replaced.
			/// \param aabb The new AABB that will be associated with the specified ID.
			/// \param force_reinsert Indicates whether or not the AABB is always reinserted,
			///						which wont happen if this is set to `true` and the new AABB is within the old AABB.
			///
			/// \return True if an AABB was updated; `false` otherwise.
			///
			[[maybe_unused]] inline const bool update(const key_type& key, math::AABB aabb, bool force_reinsert = false)
			{
				if (const auto it = m_index_map.find(key); it != m_index_map.end())
				{
					const auto node_index = it->second; // Extract the node index.

					// No need to update if the particle is still within its fattened AABB.
					if (!force_reinsert && m_nodes.at(node_index).aabb.contains(aabb))
					{
						return false;
					}

					// Remove the current leaf.
					remove_leaf(node_index);
					aabb.fatten(m_skin_thickness);

					auto& node = m_nodes.at(node_index);
					node.aabb  = aabb;
					node.aabb.update_area();

					insert_leaf(node_index);
					return true;
				}
				else
				{
					return false;
				}
			}

			///
			/// Alternate Update Function.
			///
			[[maybe_unused]] inline const bool
			update(const key_type& key, const glm::vec2& lower_bound, const glm::vec2& upper_bound, bool force_reinsert = false)
			{
				return update(key, {lower_bound, upper_bound}, force_reinsert);
			}

			///
			/// Updates the position of the AABB associated with the specified ID.
			///
			/// This function has no effect if there is no AABB associated with the specified ID.
			///
			/// \param key The ID associated with the AABB that will be moved.
			/// \param position The new position of the AABB associated with the specified ID.
			/// \param force_reinsert True if the associated AABB is forced to be reinserted into the tree.
			///
			/// \return True if an AABB was updated; `false` otherwise.
			///
			[[maybe_unused]] inline const bool relocate(const key_type& key, const glm::vec2& position, bool force_reinsert = false)
			{
				if (const auto it = m_index_map.find(key); it != m_index_map.end())
				{
					const auto& aabb = m_nodes.at(it->second).aabb;
					return update(key, {position, position + aabb.size()}, force_reinsert);
				}
				else
				{
					return false;
				}
			}

			///
			/// Rebuild an optimal tree.
			///
			inline void rebuild()
			{
				std::vector<index_type> node_indices(m_node_count);
				int count {0};

				for (auto index = 0; index < m_node_capacity; ++index)
				{
					if (m_nodes.at(index).height < 0)
					{
						// Free node.
						continue;
					}

					if (m_nodes.at(index).is_leaf())
					{
						m_nodes.at(index).parent = std::nullopt;
						node_indices.at(count)   = index;
						++count;
					}
					else
					{
						free_node(index);
					}
				}

				while (count > 1)
				{
					auto min_cost = std::numeric_limits<double>::max();
					int iMin {-1};
					int jMin {-1};

					for (auto i = 0; i < count; ++i)
					{
						const auto fst_aabb = m_nodes.at(node_indices.at(i)).aabb;

						for (auto j = (i + 1); j < count; ++j)
						{
							const auto snd_aabb = m_nodes.at(node_indices.at(j)).aabb;
							const auto cost     = math::AABB::merge(fst_aabb, snd_aabb).area();

							if (cost < min_cost)
							{
								iMin     = i;
								jMin     = j;
								min_cost = cost;
							}
						}
					}

					const auto index1 = node_indices.at(iMin);
					const auto index2 = node_indices.at(jMin);

					const auto parent_index = allocate_node();
					auto& parent_node       = m_nodes.at(parent_index);

					auto& index1Node = m_nodes.at(index1);
					auto& index2Node = m_nodes.at(index2);

					parent_node.left   = index1;
					parent_node.right  = index2;
					parent_node.height = 1 + std::max(index1Node.height, index2Node.height);
					parent_node.aabb   = math::AABB::merge(index1Node.aabb, index2Node.aabb);
					parent_node.parent = std::nullopt;

					index1Node.parent = parent_index;
					index2Node.parent = parent_index;

					node_indices.at(jMin) = node_indices.at(count - 1);
					node_indices.at(iMin) = parent_index;
					--count;
				}

				m_root = node_indices.at(0);
			}

			///
			/// Set thickness factor.
			///
			/// \param thickness_factor Skin thickness factor.
			///
			inline void set_thickness_factor(std::optional<double> thickness_factor)
			{
				if (thickness_factor)
				{
					m_skin_thickness = std::clamp(*thickness_factor, 0.0, *thickness_factor);
				}
				else
				{
					m_skin_thickness = std::nullopt;
				}
			}

			///
			/// \brief Obtains collision candidates for the AABB associated with the specified ID.
			///
			/// In order to avoid unnecessary dynamic allocations, this function
			/// returns the resulting collision candidates through an output iterator. This
			/// means that it is possible to write collision candidates to both a stack
			/// buffer and something like a `std::vector`.
			///
			/// The output iterator can for instance be obtained using
			/// `std::back_inserter`, if you're writing to a standard container.
			///
			/// This function has no effect if the supplied key is unknown.
			///
			/// \param key The ID associated with the AABB to obtain collision candidates
			/// for.
			/// \param[out] iterator The output iterator used to write the collision
			/// candidate IDs.
			///
			template<size_type buffer_size = 256, typename OutputIterator>
			inline void query(const key_type& key, OutputIterator iterator) const
			{
				if (const auto it = m_index_map.find(key); it != m_index_map.end())
				{
					const auto& source_node = m_nodes.at(it->second);

					std::array<std::byte, sizeof(maybe_index) * buffer_size> buffer;
					std::pmr::monotonic_buffer_resource resource {buffer.data(), sizeof buffer};

					pmr_stack<maybe_index> stack {&resource};
					stack.push(m_root);
					while (!stack.empty())
					{
						const auto node_index = stack.top();
						stack.pop();

						if (!node_index)
						{
							continue;
						}

						const auto& node = m_nodes.at(*node_index);

						// Test for overlap between the AABBs
						if (source_node.aabb.overlaps(node.aabb, m_touch_is_overlap))
						{
							if (node.is_leaf() && node.id)
							{
								if (node.id != key)
								{
									// Can't interact with itself
									*iterator = *node.id;
									++iterator;
								}
							}
							else
							{
								stack.push(node.left);
								stack.push(node.right);
							}
						}
					}
				}
			}

			///
			/// Compute maximum balance.
			///
			/// \return Maximum balance as a size_type.
			///
			inline [[nodiscard]] const size_type compute_maximum_balance() const
			{
				size_type max_balance {0};
				for (auto i = 0; i < m_node_capacity; ++i)
				{
					const auto& node = m_nodes.at(i);
					if (node.height > 2)
					{
						const auto bal = std::abs(m_nodes.at(*node.left).height - m_nodes.at(*node.right).height);
						max_balance    = std::max(max_balance, bal);
					}
				}

				return max_balance;
			}

			///
			/// Compute surface area.
			///
			/// \return Surface area ratio as const double.
			///
			[[nodiscard]] inline const double compute_surface_area_ratio() const
			{
				if (m_root == std::nullopt)
				{
					return 0;
				}

				const auto root_area = m_nodes.at(*m_root).aabb.compute_area();
				double total_area    = 0.0;

				for (auto i = 0; i < m_node_capacity; ++i)
				{
					const auto& node = m_nodes.at(i);

					if (node.height < 0)
					{
						continue;
					}

					total_area += node.aabb.compute_area();
				}

				return total_area / root_area;
			}

			///
			/// Returns the AABB associated with the specified ID.
			///
			/// \param key The ID associated with the desired AABB.
			///
			/// \return The AABB associated with the specified ID.
			///
			[[nodiscard]] inline const math::AABB& get_aabb(const key_type& key) const
			{
				return m_nodes.at(m_index_map.at(key)).aabb;
			}

			///
			/// Returns the current height of the tree.
			///
			/// \return The height of the tree.
			///
			[[nodiscard]] inline const int height() const
			{
				if (m_root == std::nullopt)
				{
					return 0;
				}
				else
				{
					return m_nodes.at(*m_root).height;
				}
			}

			///
			/// Returns the number of nodes in the tree.
			///
			/// \return The amount of nodes in the tree.
			///
			[[nodiscard]] inline const size_type node_count() const noexcept
			{
				return m_node_count;
			}

			///
			/// \brief Returns the amount of AABBs stored in the tree.
			///
			/// The returned value is not necessarily the amount of _nodes_ in the tree.
			///
			/// \return The current amount of AABBs stored in the tree.
			///
			[[nodiscard]] inline const size_type size() const noexcept
			{
				return m_index_map.size();
			}

			///
			/// Indicates whether or not the tree is empty.
			///
			/// \return True if there are no AABBs stored in the tree; `false` otherwise.
			///
			[[nodiscard]] inline const bool is_empty() const noexcept
			{
				return m_index_map.empty();
			}

			///
			/// Get thickness factor.
			///
			/// \return Returns thickness factor as a std::optional<double>.
			///
			[[nodiscard]] inline const std::optional<double> thickness_factor() const noexcept
			{
				return m_skin_thickness;
			}

		private:
			///
			/// Private internal function.
			///
			inline void resize_to_match_node_capacity(const size_type begin_init_index)
			{
				m_nodes.resize(m_node_capacity);
				for (auto i = begin_init_index; i < (m_node_capacity - 1); ++i)
				{
					auto& node  = m_nodes.at(i);
					node.next   = static_cast<index_type>(i) + 1;
					node.height = -1;
				}

				auto& node  = m_nodes.at(m_node_capacity - 1);
				node.next   = std::nullopt;
				node.height = -1;
			}

			///
			/// Private internal function.
			///
			inline void grow_pool()
			{
				// The free list is empty. Rebuild a bigger pool.
				m_node_capacity *= 2;
				resize_to_match_node_capacity(m_node_count);

				// Assign the index of the first free node.
				m_next_free_index = static_cast<index_type>(m_node_count);
			}

			///
			/// Private internal function.
			///
			[[nodiscard]] inline const index_type allocate_node()
			{
				if (m_next_free_index == std::nullopt)
				{
					grow_pool();
				}

				// Peel a node off the free list.
				const auto node_index = m_next_free_index.value();
				auto& node            = m_nodes.at(node_index);

				m_next_free_index = node.next;
				node.parent       = std::nullopt;
				node.left         = std::nullopt;
				node.right        = std::nullopt;
				node.height       = 0;

				++m_node_count;

				return node_index;
			}

			///
			/// Private internal function.
			///
			inline void free_node(const index_type node)
			{
				m_nodes.at(node).next   = m_next_free_index;
				m_nodes.at(node).height = -1;

				m_next_free_index = node;
				--m_node_count;
			}

			///
			/// Private internal function.
			///
			[[nodiscard]] static inline const double left_cost(const math::AABB& leaf_aabb, const node_type& left_node, const double minimum_cost)
			{
				if (left_node.is_leaf())
				{
					return math::AABB::merge(leaf_aabb, left_node.aabb).area() + minimum_cost;
				}
				else
				{
					const auto old_area = left_node.aabb.area();
					const auto new_area = math::AABB::merge(leaf_aabb, left_node.aabb).area();
					return (new_area - old_area) + minimum_cost;
				}
			}

			///
			/// Private internal function.
			///
			[[nodiscard]] static inline const double right_cost(const math::AABB& leaf_aabb, const node_type& right_node, const double minimum_cost)
			{
				if (right_node.is_leaf())
				{
					const auto aabb = math::AABB::merge(leaf_aabb, right_node.aabb);
					return aabb.area() + minimum_cost;
				}
				else
				{
					const auto aabb     = math::AABB::merge(leaf_aabb, right_node.aabb);
					const auto old_area = right_node.aabb.area();
					const auto new_area = aabb.area();
					return (new_area - old_area) + minimum_cost;
				}
			}

			///
			/// Private internal function.
			///
			[[nodiscard]] inline const index_type find_best_sibling(const math::AABB& leaf_aabb) const
			{
				auto index = m_root.value();

				while (!m_nodes.at(index).is_leaf())
				{
					const auto& node = m_nodes.at(index);
					const auto left  = node.left.value();
					const auto right = node.right.value();

					const auto surface_area          = node.aabb.area();
					const auto combined_surface_area = math::AABB::merge(node.aabb, leaf_aabb).area();

					// Cost of creating a new parent for this node and the new leaf.
					const auto cost = 2.0 * combined_surface_area;

					// Minimum cost of pushing the leaf further down the tree.
					const auto minimum_cost = 2.0 * (combined_surface_area - surface_area);

					const auto cost_left  = left_cost(leaf_aabb, m_nodes.at(left), minimum_cost);
					const auto cost_right = right_cost(leaf_aabb, m_nodes.at(right), minimum_cost);

					// Descend according to the minimum cost.
					if ((cost < cost_left) && (cost < cost_right))
					{
						break;
					}

					// Descend.
					if (cost_left < cost_right)
					{
						index = left;
					}
					else
					{
						index = right;
					}
				}

				return index;
			}

			///
			/// Private internal function.
			///
			[[nodiscard]] inline const index_type balance(const index_type node_index)
			{
				if (m_nodes.at(node_index).is_leaf() || (m_nodes.at(node_index).height < 2))
				{
					return node_index;
				}

				const auto left_index  = m_nodes.at(node_index).left.value();
				const auto right_index = m_nodes.at(node_index).right.value();

				const auto current_balance = m_nodes.at(right_index).height - m_nodes.at(left_index).height;

				// Rotate right branch up.
				if (current_balance > 1)
				{
					rotate_right(node_index, left_index, right_index);
					return right_index;
				}

				// Rotate left branch up.
				if (current_balance < -1)
				{
					rotate_left(node_index, left_index, right_index);
					return left_index;
				}

				return node_index;
			}

			///
			/// Private internal function.
			///
			inline void fix_tree_upwards(maybe_index index)
			{
				while (index != std::nullopt)
				{
					index = balance(*index);

					auto& node = m_nodes.at(*index);

					const auto left  = node.left.value();
					const auto right = node.right.value();

					const auto& left_node  = m_nodes.at(left);
					const auto& right_node = m_nodes.at(right);

					node.height = 1 + std::max(left_node.height, right_node.height);
					node.aabb   = math::AABB::merge(left_node.aabb, right_node.aabb);

					index = node.parent;
				}
			}

			///
			/// Private internal function.
			///
			inline void insert_leaf(const index_type leaf_index)
			{
				if (m_root == std::nullopt)
				{
					m_root                     = leaf_index;
					m_nodes.at(*m_root).parent = std::nullopt;
					return;
				}

				// Find the best sibling for the node.
				const auto leaf_aabb     = m_nodes.at(leaf_index).aabb; // copy current AABB
				const auto sibling_index = find_best_sibling(leaf_aabb);

				// Create a new parent.
				const auto old_parent_index = m_nodes.at(sibling_index).parent;
				const auto new_parent_index = allocate_node();

				auto& new_parent  = m_nodes.at(new_parent_index);
				new_parent.parent = old_parent_index;
				new_parent.aabb   = math::AABB::merge(leaf_aabb, m_nodes.at(sibling_index).aabb);

				new_parent.height = m_nodes.at(sibling_index).height + 1;

				if (old_parent_index != std::nullopt)
				{
					// The sibling was not the root.
					auto& old_parent = m_nodes.at(*old_parent_index);
					if (old_parent.left == sibling_index)
					{
						old_parent.left = new_parent_index;
					}
					else
					{
						old_parent.right = new_parent_index;
					}
				}
				else
				{
					// The sibling was the root.
					m_root = new_parent_index;
				}

				new_parent.left  = sibling_index;
				new_parent.right = leaf_index;

				m_nodes.at(sibling_index).parent = new_parent_index;
				m_nodes.at(leaf_index).parent    = new_parent_index;

				// Walk back up the tree fixing heights and AABBs.
				fix_tree_upwards(m_nodes.at(leaf_index).parent);
			}

			///
			/// Private internal function.
			///
			inline void adjust_ancestor_bounds(maybe_index index)
			{
				while (index != std::nullopt)
				{
					index = balance(*index);

					auto& node = m_nodes.at(*index);

					const auto left        = node.left;
					const auto right       = node.right;
					const auto& left_node  = m_nodes.at(left.value());
					const auto& right_node = m_nodes.at(right.value());

					node.aabb   = math::AABB::merge(left_node.aabb, right_node.aabb);
					node.height = 1 + std::max(left_node.height, right_node.height);

					index = node.parent;
				}
			}

			///
			/// Private internal function.
			///
			inline void remove_leaf(const index_type leaf_index)
			{
				if (leaf_index == m_root)
				{
					m_root = std::nullopt;
					return;
				}

				const auto parent_index       = m_nodes.at(leaf_index).parent;
				const auto grand_parent_index = m_nodes.at(parent_index.value()).parent;

				const auto sibling_index =
					(m_nodes.at(parent_index.value()).left == leaf_index) ? m_nodes.at(parent_index.value()).right : m_nodes.at(parent_index.value()).left;

				// Destroy the parent and connect the sibling to the grandparent.
				if (grand_parent_index != std::nullopt)
				{
					if (m_nodes.at(*grand_parent_index).left == parent_index)
					{
						m_nodes.at(*grand_parent_index).left = sibling_index;
					}
					else
					{
						m_nodes.at(*grand_parent_index).right = sibling_index;
					}

					m_nodes.at(sibling_index.value()).parent = grand_parent_index;
					free_node(parent_index.value());

					// Adjust ancestor bounds.
					adjust_ancestor_bounds(grand_parent_index);
				}
				else
				{
					m_root                                   = sibling_index;
					m_nodes.at(sibling_index.value()).parent = std::nullopt;
					free_node(parent_index.value());
				}
			}

			///
			/// Private internal function.
			///
			inline void rotate_right(const index_type node_index, const index_type left_index, const index_type right_index)
			{
				auto& node       = m_nodes.at(node_index);
				auto& right_node = m_nodes.at(right_index);

				const auto right_left  = right_node.left.value();
				const auto right_right = right_node.right.value();

				// Swap node and its right-hand child.
				right_node.left   = node_index;
				right_node.parent = node.parent;
				node.parent       = right_index;

				// The node's old parent should now point to its right-hand child.
				if (right_node.parent != std::nullopt)
				{
					auto& right_parent = m_nodes.at(*right_node.parent);
					if (right_parent.left == node_index)
					{
						right_parent.left = right_index;
					}
					else
					{
						right_parent.right = right_index;
					}
				}
				else
				{
					m_root = right_index;
				}

				auto& left_node        = m_nodes.at(left_index);
				auto& right_right_node = m_nodes.at(right_right);
				auto& right_left_node  = m_nodes.at(right_left);

				// Rotate.
				if (right_left_node.height > right_right_node.height)
				{
					right_node.right = right_left;
					node.right       = right_right;

					right_right_node.parent = node_index;

					node.aabb       = math::AABB::merge(left_node.aabb, right_right_node.aabb);
					right_node.aabb = math::AABB::merge(node.aabb, right_left_node.aabb);

					node.height       = 1 + std::max(left_node.height, right_right_node.height);
					right_node.height = 1 + std::max(node.height, right_left_node.height);
				}
				else
				{
					right_node.right = right_right;
					node.right       = right_left;

					right_left_node.parent = node_index;

					node.aabb       = math::AABB::merge(left_node.aabb, right_left_node.aabb);
					right_node.aabb = math::AABB::merge(node.aabb, right_right_node.aabb);

					node.height       = 1 + std::max(left_node.height, right_left_node.height);
					right_node.height = 1 + std::max(node.height, right_right_node.height);
				}
			}

			///
			/// Private internal function.
			///
			inline void rotate_left(const index_type node_index, const index_type left_index, const index_type right_index)
			{
				auto& node      = m_nodes.at(node_index);
				auto& left_node = m_nodes.at(left_index);

				const auto left_left  = left_node.left.value();
				const auto left_right = left_node.right.value();

				// Swap node and its left-hand child.
				left_node.left   = node_index;
				left_node.parent = node.parent;
				node.parent      = left_index;

				// The node's old parent should now point to its left-hand child.
				if (left_node.parent != std::nullopt)
				{
					auto& left_parent = m_nodes.at(*left_node.parent);
					if (left_parent.left == node_index)
					{
						left_parent.left = left_index;
					}
					else
					{
						left_parent.right = left_index;
					}
				}
				else
				{
					m_root = left_index;
				}

				auto& right_node      = m_nodes.at(right_index);
				auto& left_left_node  = m_nodes.at(left_left);
				auto& left_right_node = m_nodes.at(left_right);

				// Rotate.
				if (left_left_node.height > left_right_node.height)
				{
					left_node.right = left_left;
					node.left       = left_right;

					left_right_node.parent = node_index;

					node.aabb      = math::AABB::merge(right_node.aabb, left_right_node.aabb);
					left_node.aabb = math::AABB::merge(node.aabb, left_left_node.aabb);

					node.height      = 1 + std::max(right_node.height, left_right_node.height);
					left_node.height = 1 + std::max(node.height, left_left_node.height);
				}
				else
				{
					left_node.right = left_right;
					node.left       = left_left;

					left_left_node.parent = node_index;

					node.aabb      = math::AABB::merge(right_node.aabb, left_left_node.aabb);
					left_node.aabb = math::AABB::merge(node.aabb, left_right_node.aabb);

					node.height      = 1 + std::max(right_node.height, left_left_node.height);
					left_node.height = 1 + std::max(node.height, left_right_node.height);
				}
			}

			///
			/// Private internal function.
			///
			[[nodiscard]] inline const size_type compute_height() const
			{
				return compute_height(m_root);
			}

			///
			/// Private internal function.
			///
			[[nodiscard]] inline const size_type compute_height(const maybe_index node_index) const
			{
				if (!node_index)
				{
					return 0;
				}

				const auto& node = m_nodes.at(*node_index);
				if (node.is_leaf())
				{
					return 0;
				}
				else
				{
					const auto left  = compute_height(node.left);
					const auto right = compute_height(node.right);
					return 1 + std::max(left, right);
				}
			}

		private:
			std::vector<node_type> m_nodes;
			robin_hood::unordered_map<key_type, index_type> m_index_map;

			maybe_index m_root;
			maybe_index m_next_free_index = 0;

			size_type m_node_count = 0;
			size_type m_node_capacity;

			std::optional<double> m_skin_thickness = 0.05;

			bool m_touch_is_overlap = true;
		};
	} // namespace physics
} // namespace galaxy

#endif