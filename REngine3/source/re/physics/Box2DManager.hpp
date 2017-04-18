//
//  Box2DManager.hpp
//  REngine3
//
//  Created by reworks on 12/11/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_BOX2DMANAGER_HPP_
#define RENGINE3_BOX2DMANAGER_HPP_

#include <map>

#include "re/entity/Entity.hpp"
#include "re/types/Service.hpp"
#include "re/physics/Box2D/Box2D.h"

namespace re
{
	typedef std::map<std::pair<std::string, std::string>, std::function<void(const std::string&, const std::string&)>> CollisionFunctionMap;
	class Box2DManager : public Service
	{
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Default Constructor.
		*/
		Box2DManager(float32 gravity);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Default destructor.
		*/
		~Box2DManager();

	public:
		b2World* m_world;
		CollisionFunctionMap m_collisionFunctions;

	private:
		b2Vec2 m_gravity;
	};
}

#endif