///
///  ParticleComponent.cpp
///  starlight
///
///  Created by reworks on 14/01/2018.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#include "ParticleComponent.hpp"

namespace sl
{
	ParticleComponent::ParticleComponent(const Vector2<float>& direction, float alpha, float fade, entt::HashedString id)
		:m_direction(direction), m_alpha(alpha), m_fade(fade), m_id(id)
	{
	}

	ParticleComponent::ParticleComponent(float dx, float dy, float alpha, float fade, entt::HashedString id)
		:m_direction(dx, dy), m_alpha(alpha), m_fade(fade), m_id(id)
	{
	}
}