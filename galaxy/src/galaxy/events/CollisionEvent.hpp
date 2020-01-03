///
/// CollisionEvent.hpp
/// galaxy
///
/// Created by reworks on 15/01/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COLLISIONEVENT_HPP_
#define GALAXY_COLLISIONEVENT_HPP_

#include "galaxy/libs/entt/entity/registry.hpp"

namespace galaxy
{
	///
	/// Contains data relating to a collision event.
	///
	struct CollisionEvent final
	{
		///
		/// Constructor.
		///
		inline CollisionEvent(entt::DefaultRegistry::entity_type a, entt::DefaultRegistry::entity_type b, std::uint16_t typeA, std::uint16_t typeB) noexcept
			:m_a(a), m_b(b), m_typeA(typeA), m_typeB(typeB)
		{
		}

		///
		/// Entity A in collision.
		///
		entt::DefaultRegistry::entity_type m_a = 0;

		///
		/// Entity B in collision.
		///
		entt::DefaultRegistry::entity_type m_b = 0;

		///
		/// Fixture type A in collision.
		///
		std::uint16_t m_typeA = 0;

		///
		/// Fixture type B in collision.
		///
		std::uint16_t m_typeB = 0;
	};
}

#endif