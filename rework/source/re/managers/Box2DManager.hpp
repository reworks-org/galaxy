//
//  Box2DManager.hpp
//  REngine3
//
//  Created by reworks on 12/11/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef RENGINE3_BOX2DMANAGER_HPP_
#define RENGINE3_BOX2DMANAGER_HPP_

#include "re/types/Service.hpp"
#include "re/libs/Box2D/Box2D.h"

namespace re
{
	typedef std::map<std::pair<std::string, std::string>, std::function<void(const std::string&, const std::string&)>> CollisionFunctionMap;

	class Box2DManager : public Service
	{
	public:
		///
		/// Constructor. Sets up Box2D systems.
		///
		/// \param gravity A gravity value for Box2D to use in simulations.
		///
		Box2DManager(float32 gravity);

		///
		/// Destructor
		///
		~Box2DManager();

		///
		/// Cleans up any collision functions.
		///
		void clean();

	public:
		b2World* m_world;
		CollisionFunctionMap m_collisionFunctions;

	private:
		b2Vec2 m_gravity;
	};
}

#endif