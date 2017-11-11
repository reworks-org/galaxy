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
#include "cereal/access.hpp"

namespace re
{
	class TransformComponent
	{
		friend class cereal::access;

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

	private:
		/// This method lets cereal know which data members to serialize.
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(m_rect.x, m_rect.y, m_rect.width, m_rect.height, m_angle);
		}
	};
}

#endif