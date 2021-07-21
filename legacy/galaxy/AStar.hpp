///
/// AStar.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_AI_ASTAR_HPP_
#define GALAXY_AI_ASTAR_HPP_

#include <vector>

// https://www.redblobgames.com/pathfinding/a-star/introduction.html

namespace galaxy
{
	namespace ai
	{
		class AStar final
		{
		public:
			struct Node final
			{
				Node() noexcept;
				~Node() noexcept = default;

				[[nodiscard]] const bool operator<(const Node& node) noexcept;
				[[nodiscard]] const double total_cost() const noexcept;

				///
				/// The movement cost to move from the starting point to a given square on the grid,
				/// following the path generated to get there.
				///
				double m_movement_cost;

				///
				/// The estimated movement cost to move from that given square on the grid to the final destination.
				/// This is often referred to as the heuristic, which is nothing but a kind of smart guess.
				/// We really don’t know the actual distance until we find the path,
				/// because all sorts of things can be in the way (walls, water, etc.).
				/// There can be many ways to calculate this ‘h’ which are discussed in the later sections.
				///
				double m_estimated_cost;
			};

			AStar();
			~AStar();

			void update(const double dt);

		private:
			std::vector<Node> m_open;
			std::vector<Node> m_close;
		};
	} // namespace ai
} // namespace galaxy

#endif