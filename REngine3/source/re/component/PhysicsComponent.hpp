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

namespace re
{
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
	
	private:
		// Boost.Serialization functions
		template<class Archive>
		void save(Archive & ar, const unsigned int version) const
		{
			
		}

		template<class Archive>
		void load(Archive & ar, const unsigned int version)
		{
		
		}

		BOOST_SERIALIZATION_SPLIT_MEMBER()
	};
}

#endif
