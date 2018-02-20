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
	ParticleComponent::ParticleComponent(entt::Entity entity, const sol::table& table)
	{
		m_direction.m_x = table.get<float>("dx");
		m_direction.m_y = table.get<float>("dy");

		m_alpha = table.get<float>("alpha");
		m_fade = table.get<float>("fade");
		m_id = table.get<std::string>("id");
	}

	ParticleComponent::ParticleComponent(float dx, float dy, float alpha, float fade, const std::string& id)
		:m_direction(dx, dy), m_alpha(alpha), m_fade(fade), m_id(id)
	{
	}

	ParticleComponent& ParticleComponent::operator=(const ParticleComponent&)
	{
		return *this;
	}
}