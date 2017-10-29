//
//  RenderableComponent.hpp
//  rework
//
//  Created by reworks on 28/10/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_RENDERABLECOMPONENT_HPP_
#define REWORK_RENDERABLECOMPONENT_HPP_

#include "sol2/sol.hpp"
#include "entityx/Entity.h"

namespace re
{
	/// Just using as a flag for now.
	class RenderableComponent
	{
	public:
		///
		/// Constructor.
		///
		/// \param table sol::table containing data.
		///
		RenderableComponent(entityx::Entity& e, sol::table& table);

		///
		/// Destructor.
		///
		~RenderableComponent();

		///
		/// Calls imgui debug functions. Don't call this, done for you by debugmanager.
		///
		void debug();
	};
}

#endif