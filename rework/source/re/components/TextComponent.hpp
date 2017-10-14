//
//  TextComponent.hpp
//  rework
//
//  Created by reworks on 14/10/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_TEXTCOMPONENT_HPP_
#define REWORK_TEXTCOMPONENT_HPP_

#include "sol2/sol.hpp"

namespace re
{
	class TextComponent
	{
	public:
		///
		/// Constructor.
		///
		/// \param table sol::table containing data.
		///
		TextComponent(sol::table& table);

		///
		/// Destructor.
		///
		~TextComponent();

	public:
		unsigned int m_layer;
	};
}

#endif