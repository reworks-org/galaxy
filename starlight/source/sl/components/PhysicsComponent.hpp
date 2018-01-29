///
///  PhysicsComponent.hpp
///  starlight
///
///  Created by reworks on 10/11/2016.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_PHYSICSCOMPONENT_HPP_
#define STARLIGHT_PHYSICSCOMPONENT_HPP_

#include "cereal/access.hpp"
#include "sol2/sol_forward.hpp"

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
		/// \brief Destructor.
		///
		/// CLEANUP IS HANDLED BY BOX2D MANAGER. THIS IS BECAUSE WE DONT KNOW WHEN DESTRUCTORS ARE CALLED, AND CLEANUP CODE MUST BE CALLED BEFORE 
		/// CLEANUP OF BOX2DMANAGER!
		///
		~PhysicsComponent() = default;

	public:
		b2Body* m_body;

	private:
		/// This method lets cereal know which data members to save.
		template<class Archive>
		void save(Archive& archive) const
		{
			archive(m_body->GetPosition().x, m_body->GetPosition().y, m_body->GetAngle(), m_body->GetAngularVelocity(), m_body->GetLinearVelocity().x, m_body->GetLinearVelocity().y);
		}

		/// This method lets cereal know which data members to load.
		template<class Archive>
		void load(Archive& archive)
		{
			float32 x, y, angle, angleVel, linearX, linearY = 0.0f;

			archive(x, y, angle, angleVel, linearX, linearY);

			m_body->SetTransform(b2Vec2(x, y), angle);
			m_body->SetAngularVelocity(angleVel);
			m_body->SetLinearVelocity(b2Vec2(linearX, linearY));
		}

		///
		/// Default constructor.
		/// Deleted.
		///
		PhysicsComponent() = delete;
	};
}

#endif