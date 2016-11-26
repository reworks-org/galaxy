//
//  Box2DManager.hpp
//  REngine3
//
//  Created by reworks on 12/11/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_BOX2DMANAGER_HPP_
#define RENGINE3_BOX2DMANAGER_HPP_

#include "re/physics/Box2D/Box2D.h"

#include "re/types/Service.hpp"

namespace re
{
	class Box2DManager : public Service
	{
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Default Constructor.
		*/
		Box2DManager(double gravity);

	private:
		b2Vec2 m_gravity;

	public:
		b2World m_world;
	};
}

#endif