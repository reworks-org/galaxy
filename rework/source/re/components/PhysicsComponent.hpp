//
//  PhysicsComponent.hpp
//  rework
//
//  Created by reworks on 10/11/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_PHYSICSCOMPONENT_HPP_
#define REWORK_PHYSICSCOMPONENT_HPP_

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include "sol2/sol.hpp"
#include "Box2D/Dynamics/b2Body.h"

namespace re
{
	class PhysicsComponent
	{
		friend class boost::serialization::access;

	public:
		///
		/// Constructor.
		///
		/// \param table sol::table containing data.
		///
		PhysicsComponent(sol::table& table);

		///
		/// Destructor.
		///
		~PhysicsComponent();

	private:
		///
		/// Boost.Serialization save
		///
		template<class Archive>
		void save(Archive& ar, const unsigned int version) const
		{
			ar & m_body->GetPosition().x;
			ar & m_body->GetPosition().y;
			ar & m_body->GetAngle();
			ar & m_body->GetAngularVelocity();
			ar & m_body->GetLinearVelocity().x;
			ar & m_body->GetLinearVelocity().y;
		}

		///
		/// Boost.Serialization load
		///
		template<class Archive>
		void load(Archive& ar, const unsigned int version)
		{
			float32 x, y, angle, angleVelocity, linearX, linearY;
			ar & x;
			ar & y;
			ar & angle;
			ar & angleVelocity;
			ar & linearX;
			ar & linearY;

			m_body->SetTransform(b2Vec2(x, y), angle);
			m_body->SetAngularVelocity(angleVelocity);
			m_body->SetLinearVelocity(b2Vec2(linearX, linearY));
		}

		BOOST_SERIALIZATION_SPLIT_MEMBER()

	public:
		b2Body* m_body;
	};
}

#endif