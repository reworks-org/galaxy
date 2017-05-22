//
//  TransformComponent.hpp
//  REngine3
//
//  Created by reworks on 30/09/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_TRANSFORMCOMPONENT_HPP_
#define RENGINE3_TRANSFORMCOMPONENT_HPP_

#include <SFML/Graphics/Transformable.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include "re/types/Component.hpp"

namespace re
{
	class TransformComponent : public Component, public sf::Transformable
	{
		friend class boost::serialization::access;
        
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Default Constructor.
		*/
		TransformComponent();

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean up component.
		*/
		~TransformComponent() override;

		/*
		* IMPORTS: sol::table containing data.
		* EXPORTS: none
		* PURPOSE: Set up component.
		*/
		void init(sol::table& table) override;

		/*
		* IMPORTS: none
		* EXPORTS: Whether or not to save the changed table data.
		* PURPOSE: debug component, change data, etc.
		*/
		void debugFunction(sol::table& table) override;

	private:
		// Boost.Serialization functions
		template<class Archive>
		void save(Archive & ar, const unsigned int version) const
		{
			ar & getPosition().x;
			ar & getPosition().y;
			ar & getRotation();
		}

		template<class Archive>
		void load(Archive & ar, const unsigned int version)
		{
			float x, y, angle;
			ar & x;
			ar & y;
			ar & angle;

			setPosition(x, y);
			setRotation(angle);
		}

		BOOST_SERIALIZATION_SPLIT_MEMBER()
	};
}

#endif
