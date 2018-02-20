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

#include <cinttypes>

namespace entt { typedef std::uint32_t Entity; }

namespace sl
{
	struct CollisionEvent
	{
		///
		/// Constructor.
		///
		inline CollisionEvent(entt::Entity a, entt::Entity b, std::uint16_t typeA, std::uint16_t typeB)
			:m_a(a), m_b(b), m_typeA(typeA), m_typeB(typeB)
		{
		}

		///
		/// Entity A in collision.
		///
		entt::Entity m_a;

		///
		/// Entity B in collision.
		///
		entt::Entity m_b;

		///
		/// Fixture type A in collision.
		///
		std::uint16_t m_typeA;

		///
		/// Fixture type B in collision.
		///
		std::uint16_t m_typeB;
	};
}

#endif