///
/// Particle.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Particle.hpp"

namespace galaxy
{
	namespace graphics
	{
		Particle::Particle()
			: m_pos {0.0f}
			, m_vel {0.0f}
			, m_scale {0.1f}
			, m_colour {1.0f, 1.0f, 1.0f, 1.0f}
			, m_life {0.0f}
		{
		}
	} // namespace graphics
} // namespace galaxy
