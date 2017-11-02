//
//  CollisionEvent.hpp
//  rework
//
//  Created by reworks on 2/11/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_COLLISIONEVENT_HPP_
#define REWORK_COLLISIONEVENT_HPP_

#include "entityx/Entity.h"

namespace re
{
	class CollisionEvent
	{
	public:
		///
		/// Constructor.
		///
		/// \param left Left entity in collision.
		/// \param right Right entity in collision.
		///
		CollisionEvent(entityx::Entity& left, entityx::Entity& right);

		///
		/// \brief Handle collision between two entitys.
		///
		/// Calls collisionmap functions in Box2DHelper.
		///
		void handleCollision();

	private:
		entityx::Entity m_left;
		entityx::Entity m_right;
	};

}

#endif