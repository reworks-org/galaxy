//
//  CollisionComponent.hpp
//  rework
//
//  Created by reworks on 10/10/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_COLLISIONCOMPONENT_HPP_
#define REWORK_COLLISIONCOMPONENT_HPP_

#include "sol2/sol.hpp"

class CollisionComponent
{
public:
	///
	/// Constructor.
	///
	/// \param table sol::table containing data.
	///
	CollisionComponent(sol::table& table);

	///
	/// Destructor.
	///
	~CollisionComponent();

public:
	int m_width;
	int m_height;
};

#endif