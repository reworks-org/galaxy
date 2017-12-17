///
///  System.hpp
///  rework
///
///  Created by reworks on 15/12/2017.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef REWORK_SYSTEM_HPP_
#define REWORK_SYSTEM_HPP_

#include <cstdint>

#include "entt/entt.hpp"

namespace re
{
	class System
	{
	public:
		///
		/// Virtual destructor.
		virtual inline ~System() = default;

		///
		/// Update method.
		///
		/// \param dt Delta Time.
		///
		virtual void update(const double dt, entt::DefaultRegistry& registery) = 0;

	public:
		static std::uint32_t m_id;
	};
}

#endif