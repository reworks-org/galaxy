///
/// System.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SYSTEMS_SYSTEM_HPP_
#define GALAXY_SYSTEMS_SYSTEM_HPP_

#include <concepts>

#include <entt/core/family.hpp>
#include <entt/entity/registry.hpp>

namespace galaxy
{
	namespace systems
	{
		///
		/// Represents a system that operates on sets of components.
		///
		class System
		{
		public:
			///
			/// Destructor.
			///
			virtual ~System();

			///
			/// Abstract implementation for updating the system. Use the manager to retreive your components.
			///
			/// \param EnTT registry to process entities from.
			///
			virtual void update(entt::registry& registry) = 0;

		protected:
			///
			/// Constructor.
			///
			System();
		};
	} // namespace systems

	namespace meta
	{
		///
		/// Concept to ensure a system is actually derived from a System.
		///
		/// \tparam Type Type to test.
		///
		template<typename Type>
		concept is_system = std::derived_from<Type, systems::System>;
	} // namespace meta
} // namespace galaxy

#endif
