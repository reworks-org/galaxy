///
/// CollisionEvent.hpp
/// starlight
///
/// Created by reworks on 15/01/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_COLLISIONEVENT_HPP_
#define STARLIGHT_COLLISIONEVENT_HPP_

struct CollisionEvent
{
	///
	/// Entity A in collision.
	/// We use the non typedef of entt::Entity because it saves us having to include a bunch of headers...
	///
	unsigned int m_entityA;

	///
	/// Entity B in collision.
	/// We use the non typedef of entt::Entity because it saves us having to include a bunch of headers...
	///
	unsigned int m_entityB;
};

#endif