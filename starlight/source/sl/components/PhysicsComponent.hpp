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

#include "sl/libs/entt/entity/registry.hpp"

class b2Body;

namespace sl
{
	class PhysicsComponent final
	{
		friend class cereal::access;

	public:
		///
		/// Constructor.
		///
		/// \param table sol::table containing data.
		///
		PhysicsComponent(const sol::table& table);

		///
		/// Destructor.
		///
		~PhysicsComponent();

		///
		/// Default move assignment overload.
		///
		PhysicsComponent& operator=(const PhysicsComponent&);

		///
		/// Set the correct entity to the fixture data.
		///
		/// \param entity Entity this component belongs to.
		///
		void setFixtureEntity(entt::DefaultRegistry::entity_type entity);

	private:
		///
		/// Default constructor.
		/// Deleted.
		///
		PhysicsComponent() = delete;

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
		b2Body* m_body;

	private:
		entt::DefaultRegistry::entity_type m_entity;
	};
}

#endif