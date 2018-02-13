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

namespace entt { typedef std::uint32_t Entity }

struct CollisionEvent
{
	///
	/// Constructor.
	///
	inline CollisionEvent(entt::Entity a, entt::Entity b)
	:m_entityA(a), m_entityB(b)
	{
	}

	///
	/// Entity A in collision.
	///
	entt::Entity m_entityA;

	///
	/// Entity B in collision.
	///
	entt::Entity m_entityB;
};

#endif