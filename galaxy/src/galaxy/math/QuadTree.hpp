///
/// QuadTree.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MATH_QUADTREE_HPP_
#define GALAXY_MATH_QUADTREE_HPP_

#include <array>
#include <memory>
#include <vector>

#include "galaxy/ecs/Entity.hpp"
#include "galaxy/math/AABB.hpp"
#include "galaxy/math/Rect.hpp"

namespace galaxy
{
	namespace math
	{
		///
		/// 2D spacial partitioning.
		///
		class Quadtree final
		{
		public:
			///
			/// Entity with AABB.
			///
			struct Object final
			{
				///
				/// Entity AABB from Renderable component.
				///
				AABB* m_aabb;

				///
				/// Entity AABB belongs to.
				///
				ecs::Entity m_entity;
			};

			///
			/// Argument constructor.
			///
			/// \param level Level of this quadtree. First is 0.
			/// \param bounds Quadtree bounds.
			/// \param max_objects Optional. Max objects in a quadtree before splitting.
			/// \param max_levels Optional. Total number of levels allowed in a quadtree.
			///
			Quadtree(const int level, const Rect<float>& bounds, int max_objects = 10, int max_levels = 5) noexcept;

			///
			/// Destructor.
			///
			~Quadtree() noexcept;

			///
			/// Resize quadtree bounds.
			///
			/// \param width New quadtree width.
			/// \param height New quadtree height.
			///
			void resize(const int width, const int height) noexcept;

			///
			/// \brief Insert an object into the quadtree.
			///
			/// If the node exceeds the capacity, it will split and add all objects to their corresponding nodes.
			///
			/// \param object Object to be inserted.
			///
			void insert(const Quadtree::Object& object);

			///
			/// Return all entities in the same spacial partition.
			///
			/// \param object Object to check.
			/// \param output Array of objects that are in the same spacial partition.
			///
			void query(const Quadtree::Object& object, std::vector<Quadtree::Object*>& output);

			///
			/// Clears the quadtree.
			///
			void clear();

		private:
			///
			/// Constructor.
			///
			Quadtree() = delete;

			///
			/// Splits the quadtree into 4 sub-trees.
			///
			void split();

			///
			/// Determine which node the object belongs to.
			///
			/// \param object Object to query its spacial index.
			///
			/// \return Index of object. -1 means object cannot completely fit within a child tree and is part of the parent tree.
			///
			[[nodiscard]] const int get_index(const Quadtree::Object& object);

		private:
			///
			/// Current tree level.
			///
			int m_level;

			///
			/// Tree bounds.
			///
			Rect<float> m_bounds;

			///
			/// Max objects in a partition.
			///
			int m_max_objects;

			///
			/// Max levels in the tree.
			///
			int m_max_levels;

			///
			/// List of stored objects.
			///
			std::vector<Quadtree::Object> m_objects;

			///
			/// Sub-trees.
			///
			std::array<std::unique_ptr<Quadtree>, 4> m_nodes;
		};
	} // namespace math
} // namespace galaxy

#endif