//
//  AnimationSystem.hpp
//  rework
//
//  Created by reworks on 10/11/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_ANIMATIONSYSTEM_HPP_
#define REWORK_ANIMATIONSYSTEM_HPP_

#include "entityx/System.h"

namespace re
{
	class AnimationSystem : public entityx::System<AnimationSystem>
	{
	public:
		///
		/// Constructor.
		///
		AnimationSystem();

		///
		/// Destructor
		///
		~AnimationSystem() override;

		///
		/// \brief Update the system.
		///
		/// Dont actually call this, this is called by entity x internal system manager.
		///
		void update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt) override;
	};
}

#endif