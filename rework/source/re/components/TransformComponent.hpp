///
///  TransformComponent.hpp
///  rework
///
///  Created by reworks on 30/09/2016.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef REWORK_TRANSFORMCOMPONENT_HPP_
#define REWORK_TRANSFORMCOMPONENT_HPP_

#include "re/math/Rect.hpp"
#include "cereal/access.hpp"
#include "sol2/sol_forward.hpp"

namespace re
{
	class TransformComponent final
	{
		friend class cereal::access;

	public:
		///
		/// Constructor.
		///
		/// \param table sol::table containing data.
		///
		TransformComponent(const sol::table& table);

		///
		/// Move Constructor.
		///
		TransformComponent(TransformComponent&&) = default;

		///
		/// Destructor.
		///
		~TransformComponent() = default;

		///
		/// Calls imgui debug functions. Don't call this, done for you by debugmanager.
		///
		void debug();

	public:
		float m_angle;
		Rect<float, int> m_rect;

	private:
		/// This method lets cereal know which data members to serialize.
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(m_rect.x, m_rect.y, m_rect.width, m_rect.height, m_angle);
		}
		
		///
		/// Default constructor.
		/// Deleted.
		///
		TransformComponent() = delete;

		///
		/// Copy Constructor.
		/// Deleted.
		///
		TransformComponent(const TransformComponent&) = delete;
	};
}

#endif