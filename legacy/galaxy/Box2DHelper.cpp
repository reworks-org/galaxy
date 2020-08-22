///
/// Box2DManager.cpp
/// galaxy
///
/// Created by reworks on 12/11/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "Box2DHelper.hpp"

namespace galaxy
{
	Box2DHelper::Box2DHelper()
		:m_b2world(std::make_unique<b2World>(b2Vec2(0.0f, 9.81f)))
	{
		// Create the Box2D physics world.
	}

	Box2DHelper::Box2DHelper(float32 gravity)
		:m_b2world(std::make_unique<b2World>(b2Vec2(0.0f, gravity)))
	{
		// Create the Box2D physics world.
	}

	Box2DHelper::~Box2DHelper()
	{
		// Ensure Box2D is cleaned up.
		m_b2world.reset();
	}
}