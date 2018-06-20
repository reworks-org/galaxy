///
/// Box2DManager.cpp
/// starlight
///
/// Created by reworks on 12/11/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "Box2DHelper.hpp"

namespace sl
{
	Box2DHelper::Box2DHelper()
	{
		// Create the Box2D physics world.
		m_b2world = std::make_unique<b2World>(b2Vec2(0.0f, 9.81f));
	}

	Box2DHelper::Box2DHelper(float32 gravity)
	{
		// Create the Box2D physics world.
		m_b2world = std::make_unique<b2World>(b2Vec2(0.0f, gravity));
	}

	Box2DHelper::~Box2DHelper()
	{
		// Ensure Box2D is cleaned up.
		m_b2world.reset();
	}
}