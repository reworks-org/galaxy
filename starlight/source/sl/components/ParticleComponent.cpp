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
	ParticleComponent::ParticleComponent(const Vector2<float>& position, const Vector2<float>& direction, float alpha, entt::HashedString id)
		:m_position(position), m_direction(direction), m_alpha(alpha), m_id(id)
	{
	}

	ParticleComponent::ParticleComponent(float px, float py, float dx, float dy, float alpha, entt::HashedString id)
		:m_position(px, py), m_direction(dx, dy), m_alpha(alpha), m_id(id)
	{
	}
}