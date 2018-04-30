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

#include "sl/libs/entt/entity/registry.hpp"

namespace sl
{
	struct CollisionEvent
	{
		///
		/// Constructor.
		///
		inline CollisionEvent(entt::DefaultRegistry::entity_type a, entt::DefaultRegistry::entity_type b, std::uint16_t typeA, std::uint16_t typeB)
			:m_a(a), m_b(b), m_typeA(typeA), m_typeB(typeB)
		{
		}

		///
		/// Entity A in collision.
		///
		entt::DefaultRegistry::entity_type m_a;

		///
		/// Entity B in collision.
		///
		entt::DefaultRegistry::entity_type m_b;

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