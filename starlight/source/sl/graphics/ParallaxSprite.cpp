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
	ParallaxSprite::ParallaxSprite(int layer, float speed, float posX, float posY, const char* id)
		:m_layer(layer), m_speed(speed), m_position(posX, posY), m_atlasID(id)
	{
	}
}