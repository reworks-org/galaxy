///
///  System.hpp
///  starlight
///
///  Created by reworks on 15/12/2017.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef REWORK_SYSTEM_HPP_
#define REWORK_SYSTEM_HPP_

#include <cstdint>

#include "entt/entity/registry.hpp"

namespace sl
{
	class System
	{
		friend class World;
	public:
		///
		/// Virtual destructor.
		///
		virtual ~System() = default;

	protected:
		///
		/// Update method.
		///
		/// \param dt Delta Time.
		///
		virtual void update(const double dt, entt::DefaultRegistry& registery) = 0;

	protected:
		static std::uint32_t m_id;
	};
}

#endif