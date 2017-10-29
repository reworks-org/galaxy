//
//  TransformComponent.hpp
//  rework
//
//  Created by reworks on 30/09/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_TRANSFORMCOMPONENT_HPP_
#define REWORK_TRANSFORMCOMPONENT_HPP_

#include "sol2/sol.hpp"
#include "entityx/Entity.h"
#include "re/math/Rect.hpp"

namespace re
{
	class TransformComponent
	{
	public:
		///
		/// Constructor.
		///
		/// \param table sol::table containing data.
		///
		TransformComponent(entityx::Entity& e, sol::table& table);

		///
		/// Destructor.
		///
		~TransformComponent();

		///
		/// Calls imgui debug functions. Don't call this, done for you by debugmanager.
		///
		void debug();

	public:
		Rect<float, int> m_rect;
		float m_angle;
	};
}

#endif