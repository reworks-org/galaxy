//
//  Camera.hpp
//  rework
//
//  Created by reworks on 28/10/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_CAMERA_HPP_
#define REWORK_CAMERA_HPP_

#include "entityx/Entity.h"
#include "re/mapping/Level.hpp"

namespace re
{
	class Camera
	{
		///
		/// Constructor.
		///
		/// \param e Entity for the camera to follow. Entity MUST have a transform component.
		///
		Camera(entityx::Entity e);

		///
		/// Constructor.
		///
		/// \param bounds Bounds for the camera to follow.
		///
		Camera(Rect<float, int> bounds);

		///
		/// Constructor.
		///
		/// \param level 
		///
		void update(Level* level);

	private:
		entityx::Entity m_entityToFollow;
		Rect<float, int> m_bounds;
	};
}

#endif