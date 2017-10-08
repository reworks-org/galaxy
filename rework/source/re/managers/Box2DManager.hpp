//
//  Box2DManager.hpp
//  rework
//
//  Created by reworks on 12/11/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_BOX2DMANAGER_HPP_
#define REWORK_BOX2DMANAGER_HPP_

#include <map>
#include <functional>

#include "Box2D/Box2D.h"
#include "re/types/Service.hpp"

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
		/// Removes any collision functions in the map.
		///
		void clean();

		///
		/// Access the Box2D world.
		///
		/// \return Returns pointer to Box2D world.
		///
		b2World* world();

		///
		/// Access the map containing functions to execute on collision.
		///
		/// \return Returns reference to collision function map.
		///
		CollisionFunctionMap& getCollisionFunctions();

	private:
		b2World* m_world;
		CollisionFunctionMap m_collisionFunctions;
	};
}

#endif