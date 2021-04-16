///
/// Spot.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Spot.hpp"

namespace galaxy
{
	namespace light
	{
		Spot::Spot() noexcept
		    : Point {}, m_dir {0.0f, 0.0f, 0.0f}, m_inner_cutoff {0.91f}, m_outer_cutoff {0.82f}
		{
		}
	} // namespace light
} // namespace galaxy