///
/// Algorithms.cpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#include <cmath>

#include "Algorithms.hpp"

///
/// Core namespace.
///
namespace rs
{
	///
	/// Grouping for collision algorithms.
	///
	namespace collisions
	{
		const bool circle_circle(const pr::Circle& a, const pr::Circle& b) noexcept
		{
			const glm::vec2 combined_pos = a.get_xy() + b.get_xy();
			float total_radius           = a.radius() + b.radius();
			total_radius *= total_radius;

			return total_radius < static_cast<float>(std::pow(combined_pos.x, 2) + std::pow(combined_pos.y, 2));
		}
	} // namespace collisions
} // namespace rs