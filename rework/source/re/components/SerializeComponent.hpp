//
//  SerializeComponent.hpp
//  rework
//
//  Created by reworks on 21/10/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_SERIALIZECOMPONENT_HPP_
#define REWORK_SERIALIZECOMPONENT_HPP_

#include "sol2/sol.hpp"

namespace re
{
	class SerializeComponent
	{
	public:
		///
		/// Constructor.
		///
		/// \param table sol::table containing data.
		///
		SerializeComponent(sol::table& table);

		///
		/// Destructor.
		///
		~SerializeComponent();

	public:
		std::string m_id;
	};
}

#endif