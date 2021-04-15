///
/// Directional.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Directional.hpp"

namespace galaxy
{
	namespace light
	{
		Directional::Directional() noexcept
		    : Light {}, m_dir {-0.2f, -1.0f, -0.3f}
		{
		}
	} // namespace light
} // namespace galaxy