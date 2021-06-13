///
/// QuadTree.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_ALGORITHM_QUADTREE_HPP_
#define GALAXY_ALGORITHM_QUADTREE_HPP_

namespace galaxy
{
	namespace algorithm
	{
		///
		/// 2D spacial partitioning.
		///
		class QuadTree final
		{
		public:
			///
			/// A node in the tree.
			///
			class Node final
			{
				///
				/// First child node (branch) or
				/// First element (leaf).
				///
				int m_first_child;

				///
				/// Number of elements in the leaf.
				/// -1 if not a leaf.
				///
				int m_count;
			};
		};
	} // namespace algorithm
} // namespace galaxy

#endif