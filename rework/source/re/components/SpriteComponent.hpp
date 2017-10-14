//
//  SpriteComponent.hpp
//  rework
//
//  Created by reworks on 12/08/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_SPRITECOMPONENT_HPP_
#define REWORK_SPRITECOMPONENT_HPP_

#include "sol2/sol.hpp"

namespace re
{
	class SpriteComponent
	{
	public:
		///
		/// Constructor.
		///
		/// \param table sol::table containing data.
		///
		SpriteComponent(sol::table& table);

		///
		/// Destructor.
		///
		~SpriteComponent();

	public:
		unsigned int m_layer;
		std::string m_spriteName;
	};
}

#endif
