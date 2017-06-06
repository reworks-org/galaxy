//
//  PhysicsComponent.hpp
//  REngine3
//
//  Created by reworks on 10/11/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_PHYSICSCOMPONENT_HPP_
#define RENGINE3_PHYSICSCOMPONENT_HPP_

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include "re/types/Component.hpp"
#include "re/physics/Box2D/Dynamics/b2Body.h"

namespace re
{
	struct PhysicsFixtureUserData
	{
		PhysicsFixtureUserData(const std::string& str)
		{
			m_str = str;
		}

		std::string m_str = "";
	};

	class PhysicsComponent : public Component
	{
		friend class boost::serialization::access;

	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Default Constructor.
		*/
		PhysicsComponent();

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean up component.
		*/
		~PhysicsComponent() override;

		/*
		* IMPORTS: sol::table containing data.
		* EXPORTS: none
		* PURPOSE: Set up component.
		*/
		void init(sol::table& table) override;

		/*
		* IMPORTS: lua table and entity name
		* EXPORTS: Whether or not to save the changed table data.
		* PURPOSE: debug component, change data, etc.
		*/
		void debugFunction(sol::table& table, const std::string& curEntityName) override;

	public:
		b2Body* m_body;
		bool m_isMovingVertically = false;
		bool m_isMovingHorizontally = false;

	private:
		// Boost.Serialization functions
		template<class Archive>
		void save(Archive & ar, const unsigned int version) const
		{
			ar & m_body->GetPosition().x;
			ar & m_body->GetPosition().y;
			ar & m_body->GetAngle();
			ar & m_body->GetAngularVelocity();
			ar & m_body->GetLinearVelocity().x;
			ar & m_body->GetLinearVelocity().y;
			ar & m_isMovingVertically;
			ar & m_isMovingHorizontally;
		}

		template<class Archive>
		void load(Archive & ar, const unsigned int version)
		{
			float32 x, y, angle, angleVelocity, linearX, linearY;
			ar & x;
			ar & y;
			ar & angle;
			ar & angleVelocity;
			ar & linearX;
			ar & linearY;
			ar & m_isMovingVertically;
			ar & m_isMovingHorizontally;

			m_body->SetTransform(b2Vec2(x, y), angle);
			m_body->SetAngularVelocity(angleVelocity);
			m_body->SetLinearVelocity(b2Vec2(linearX, linearY));
		}

		BOOST_SERIALIZATION_SPLIT_MEMBER()
	};
}

#endif
