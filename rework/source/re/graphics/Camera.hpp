//
//  Camera.hpp
//  rework
//
//  Created by reworks on 28/10/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_CAMERA_HPP_
#define REWORK_CAMERA_HPP_

#include "re/math/Rect.hpp"
#include "re/types/Service.hpp"

namespace re
{
	struct Camera : public Service
	{
		Camera(Rect<float, int> bounds);

		Rect<float, int> m_bounds;
	};
}

#endif