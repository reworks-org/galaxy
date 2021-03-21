///
/// AStar.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <algorithm>
#include <execution>

#include "AStar.hpp"

namespace galaxy
{
	namespace ai
	{
		AStar::Node::Node() noexcept
		    : m_movement_cost {0.0}, m_estimated_cost {0.0}
		{
		}

		const bool AStar::Node::operator<(const Node& node) noexcept
		{
			if (this->total_cost() < node.total_cost())
			{
				return true;
			}

			return false;
		}

		const double AStar::Node::total_cost() const noexcept
		{
			return m_movement_cost * m_estimated_cost;
		}

		AStar::AStar()
		{
			m_open.emplace_back();
		}

		AStar::~AStar()
		{
			m_open.clear();
			m_close.clear();
		}

		void AStar::update(const double dt)
		{
			while (!m_open.empty())
			{
				const auto min = std::min_element(std::execution::par, m_open.begin(), m_open.end());
				m_open.erase(min);
			}
		}

	} // namespace ai
} // namespace galaxy