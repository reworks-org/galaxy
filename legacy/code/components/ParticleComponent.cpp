///
/// ParticleComponent.cpp
/// galaxy
///
/// Created by reworks on 14/01/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/libs/sol2/sol.hpp"

#include "ParticleComponent.hpp"

namespace galaxy
{
	ParticleComponent::ParticleComponent()
		:m_fade(0.0f), m_direction(0.0f, 0.0f)
	{
	}

	ParticleComponent::ParticleComponent(const sol::table& table)
	{
		// Retrieve the values from the lua table.
		m_direction.m_x = table.get<float>("dx");
		m_direction.m_y = table.get<float>("dy");

		m_fade = table.get<float>("fade");
	}

	ParticleComponent::ParticleComponent(const float dx, const float dy, const float fade)
		:m_direction(dx, dy), m_fade(fade)
	{
	}
}