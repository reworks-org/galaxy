//
//  CollisionComponent.cpp
//  REngine3
//
//  Created by reworks on 28/08/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef RENGINE3_COLLISIONCOMPONENT_HPP_
#define RENGINE3_COLLISIONCOMPONENT_HPP_

#include "re/math/Rect.hpp"
#include "re/types/Component.hpp"

namespace re
{
	class CollisionComponent : public Component
	{
	public:
		///
		/// Default Constructor
		///
		/// \param table sol::table containing data.
		///
		CollisionComponent(sol::table& table);

		///
		/// Destructor.
		///
		~CollisionComponent() override;
		
	private:
		///
		/// Debug function called by debug manager.
		///
		void debug(sol::table& table, const std::string& curEntityName) override;

	public:
		Rect<int> m_rect;
	};
}

#endif
