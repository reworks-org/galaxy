///
/// ParticleComponent.cpp
/// starlight
///
/// Created by reworks on 14/01/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/libs/sol2/sol.hpp"

#include "ParticleComponent.hpp"

namespace sl
{
	ParticleComponent::ParticleComponent(const sol::table& table)
	{
		// Retrieve the values from the lua table.
		m_direction.m_x = table.get<float>("dx");
		m_direction.m_y = table.get<float>("dy");

		m_fade = table.get<float>("fade");
	}

	ParticleComponent::ParticleComponent(float dx, float dy, float fade)
		:m_direction(dx, dy), m_fade(fade)
	{
		// Constructs the component from parameters instead of lua.
	}

	ParticleComponent& ParticleComponent::operator=(const ParticleComponent&)
	{
		return *this;
	}
}