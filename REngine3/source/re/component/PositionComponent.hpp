//
//  PositionComponent.hpp
//  REngine3
//
//  Created by reworks on 30/09/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_POSITIONCOMPONENT_HPP_
#define RENGINE3_POSITIONCOMPONENT_HPP_

#include <SFML/Graphics/Transformable.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include "re/types/Component.hpp"

namespace re
{
	class PositionComponent : public Component, public sf::Transformable
	{
		friend class boost::serialization::access;
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Default Constructor.
		*/
		PositionComponent();

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean up component.
		*/
		~PositionComponent() override;

		/*
		* IMPORTS: sol::table containing data.
		* EXPORTS: none
		* PURPOSE: Set up component.
		*/
		void Init(sol::table& table) override;

		/*
		* IMPORTS: new x and y coords.
		* EXPORTS: none
		* PURPOSE: Easy method to update positions. USE THIS NOT setPosition()!
		*/
		void SetPos(float x, float y);

	public:
		float m_xpos;
		float m_ypos;
		float m_width;
		float m_height;
		bool m_isColliding = false;

	private:
		// Boost.Serialization function
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & m_xpos;
			ar & m_ypos;
			
			setPosition(m_xpos, m_ypos);
		}
	};
}

#endif