//
//  SerializeComponent.hpp
//  rework
//
//  Created by reworks on 21/10/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_SERIALIZECOMPONENT_HPP_
#define REWORK_SERIALIZECOMPONENT_HPP_

namespace re
{
	class SerializeComponent
	{
	public:
		SerializeComponent(sol::table& table);
		~SerializeComponent();

		void save(ex::Entity& e);
		void load(ex::Entity& e);

	private:
		std::string m_id;
	};
}

#endif