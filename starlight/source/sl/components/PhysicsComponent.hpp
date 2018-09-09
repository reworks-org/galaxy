///
/// PhysicsComponent.hpp
/// starlight
///
/// Created by reworks on 10/11/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_PHYSICSCOMPONENT_HPP_
#define STARLIGHT_PHYSICSCOMPONENT_HPP_

#include "sl/libs/cereal/access.hpp"
#include "sl/libs/sol2/sol_forward.hpp"
#include "sl/libs/Box2D/Dynamics/b2Body.h"
#include "sl/libs/entt/entity/registry.hpp"
#include "sl/libs/entt/entity/snapshot.hpp"

namespace sl
{
	///
	/// Controls an entitys physics.
	///
	class PhysicsComponent final
	{
		friend class cereal::access;
	public:
		///
		/// \brief Default Constructor.
		///
		/// Contains empty data values.
		///
		PhysicsComponent();

		///
		/// Constructor.
		///
		/// \param table sol::table containing data.
		///
		explicit PhysicsComponent(const sol::table& table);

		///
		/// \brief Destructor. See desc for important info.
		///
		// The manager iterates over all physics components and destroys the bodies.
		// Eww coupling but it is a workaround for a wierd behaviour where entt seems to destroy components as its assigning the component.
		// So what would happen is the body data would be freed causing a string of exceptions when dat that no longer exists gets accessed.
		///
		~PhysicsComponent() = default;

		///
		/// Set the correct entity to the fixture data.
		///
		/// \param entity Entity this component belongs to.
		///
		void setFixtureEntity(entt::DefaultRegistry::entity_type entity);

	private:
		///
		/// Cereal serialize function.
		///
		template<class Archive>
		void save(Archive& ar) const
		{
			ar(m_body->GetPosition().x, m_body->GetPosition().y, m_body->GetAngle(), m_body->GetAngularVelocity(), m_body->GetLinearVelocity().x, m_body->GetLinearVelocity().y);
		}

		///
		/// Cereal deserialize function.
		///
		template<class Archive>
		void load(Archive& ar)
		{
			float32 x, y, angle, angleVel, linearX, linearY = 0.0f;

			ar(x, y, angle, angleVel, linearX, linearY);

			m_body->SetTransform(b2Vec2(x, y), angle);
			m_body->SetAngularVelocity(angleVel);
			m_body->SetLinearVelocity(b2Vec2(linearX, linearY));
		}

	public:
		///
		/// Pointer to the Box2D physics body data.
		///
		b2Body* m_body;

	private:
		///
		/// We need to reference the entity this component belongs to in order for collisions to know 'what' collided.
		///
		entt::DefaultRegistry::entity_type m_entity;
	};
}

#endif