///
/// System.hpp
/// starlight
///
/// Created by reworks on 15/12/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_SYSTEM_HPP_
#define STARLIGHT_SYSTEM_HPP_

#include "sl/libs/entt/entity/registry.hpp"

namespace sl
{
	///
	/// Represets a "system" which is a class that process's specific type(s) of component(s).
	///
	class System
	{
		friend class World;
	public:
		///
		/// Virtual destructor.
		///
		virtual ~System() noexcept = default;
		
	protected:
		///
		/// Default constructor.
		///
		System() = default;

		///
		/// Update method.
		///
		/// \param dt Delta Time.
		/// \param registry The registry used by the World class which contains all the application entities.
		///
		virtual void update(const double dt, entt::DefaultRegistry& registry) = 0;
	};
}

#endif