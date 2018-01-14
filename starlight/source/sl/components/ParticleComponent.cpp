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
	ParticleComponent::ParticleComponent(float x, float y, float alpha, entt::HashedString id)
	:m_x(x), m_y(y), m_alpha(alpha), m_atlasID(id)
	{
	}

	void ParticleComponent::debug()
	{
		// to do
	}
}