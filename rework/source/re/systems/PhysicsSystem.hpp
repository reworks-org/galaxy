//
//  PhysicsSystem.hpp
//  rework
//
//  Created by reworks on 8/11/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_PHYSICSSYSTEM_HPP_
#define REWORK_PHYSICSSYSTEM_HPP_

#include "entityx/System.h"

namespace re
{
	class Box2DManager;

	class PhysicsSystem : public entityx::System<PhysicsSystem>
	{
	public:
		///
		/// Construct the system.
		///
		/// \param ups Updates per second for physics system.
		/// \param vi Box2D velocity iterations.
		/// \param pi Box2D position iterations.
		///
		PhysicsSystem(float ups, int32 vi, int32 pi);

		///
		/// Destructor.
		///
		~PhysicsSystem() override;
		
		///
		/// \brief Update the system.
		///
		/// Dont actually call this, this is called by entity x internal system manager.
		///
		void update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt) override;

	private:
		float m_ups;
		int32 m_velocityIterations;
		int32 m_positionIterations;

		Box2DManager* m_manager;
	};
}

#endif