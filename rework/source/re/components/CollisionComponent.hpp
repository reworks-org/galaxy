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

namespace ex
{
	class Entity;
}

namespace re
{
	class CollisionComponent
	{
	public:
		///
		/// Constructor.
		///
		/// \param table sol::table containing data.
		///
		CollisionComponent(ex::Entity& e, sol::table& table);

		///
		/// Destructor.
		///
		~CollisionComponent();

		///
		/// Calls imgui debug functions. Don't call this, done for you by debugmanager.
		///
		void debug();

	public:
		int m_width;
		int m_height;
	};
}

#endif