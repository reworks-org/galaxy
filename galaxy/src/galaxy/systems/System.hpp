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

namespace galaxy
{
	namespace state
	{
		class Layer;
	} // namespace state

	namespace systems
	{
		///
		/// Represents a system that operates on sets of components.
		///
		class System
		{
		public:
			using Id = entt::family<System>;

			///
			/// Destructor.
			///
			virtual ~System() noexcept;

			///
			/// Abstract implementation for updating the system. Use the manager to retreive your components.
			///
			/// \param layer Non-owning pointer to current layer.
			///
			virtual void update(state::Layer* layer) = 0;

		protected:
			///
			/// Constructor.
			///
			System() noexcept;
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