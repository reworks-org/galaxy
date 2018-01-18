///
/// ParallaxSprite.cpp
/// starlight
///
/// Created by reworks on 18/01/2018.
/// Copyright (c) 2018+ reworks.
/// Refer to LICENSE.txt for more details.
///

#include "ParallaxSprite.hpp"

namespace sl
{
	ParallaxSprite::ParallaxSprite(float velX, float velY, float speed, entt::HashedString id)
		:m_velocity(velX, velY), m_speed(speed), m_atlasID(id)
	{
	}
}