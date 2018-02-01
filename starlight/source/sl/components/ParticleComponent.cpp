///
/// ParticleComponent.cpp
/// starlight
///
/// Created by reworks on 14/01/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "ParticleComponent.hpp"

namespace sl
{
	ParticleComponent::ParticleComponent(float dx, float dy, float alpha, float fade, const std::string& id)
		:m_direction(dx, dy), m_alpha(alpha), m_fade(fade), m_id(id)
	{
	}

	ParticleComponent& ParticleComponent::operator=(const ParticleComponent &)
	{
		return *this;
	}
}