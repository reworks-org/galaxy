//
//  ParallaxComponent.hpp
//  rework
//
//  Created by reworks on 29/08/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_PARALLAXCOMPONENT_HPP_
#define REWORK_PARALLAXCOMPONENT_HPP_

#include "sol2/sol.hpp"
#include "re/types/Renderable.hpp"
#include "re/graphics/ParallaxBitmap.hpp"

namespace re
{
	class ParallaxComponent
	{
	public:
		///
		/// Constructor.
		///
		/// \param table sol::table containing data.
		///
		ParallaxComponent(entityx::Entity& e, sol::table& table);

		///
		/// Destructor.
		///
		~ParallaxComponent();
		
		///
		/// Calls imgui debug functions. Don't call this, done for you by debugmanager.
		///
		void debug();

	public:
		std::vector<ParallaxBitmap> m_pb;
	};
}

#endif