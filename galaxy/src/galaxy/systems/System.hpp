///
/// System.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SYSTEMS_SYSTEM_HPP_
#define GALAXY_SYSTEMS_SYSTEM_HPP_

#include <concepts>
#include <string>

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
			virtual ~System() noexcept;

			///
			/// Abstract implementation for updating the system. Use the manager to retreive your components.
			///
			/// \param EnTT registry to process entities from.
			///
			virtual void update(entt::registry& registry) = 0;

			///
			/// Get identifier.
			///
			/// \return Const string reference.
			///
			[[nodiscard]]
			virtual const std::string& id() const noexcept final;

		protected:
			///
			/// Constructor.
			///
			/// \param id System debug identifier.
			///
			System(const std::string& id) noexcept;

		private:
			///
			/// Constructor.
			///
			System() = delete;

		protected:
			///
			/// Debug id.
			///
			std::string m_id;
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
