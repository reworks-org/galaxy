///
/// SystemFactory.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_META_SYSTEMFACTORY_HPP_
#define GALAXY_META_SYSTEMFACTORY_HPP_

#include <ankerl/unordered_dense.h>

#include "galaxy/math/FNV1a.hpp"
#include "galaxy/meta/Concepts.hpp"
#include "galaxy/logging/Log.hpp"

namespace galaxy
{
	namespace systems
	{
		class System;
	} // namespace systems

	namespace meta
	{
		using SystemStack = std::vector<std::unique_ptr<systems::System>>;

		///
		/// Meta factory for creating system objects.
		///
		class SystemFactory final
		{
		public:
			///
			/// Constructor.
			///
			SystemFactory() noexcept = default;

			///
			/// Destructor.
			///
			~SystemFactory() noexcept = default;

			///
			/// Registers a system definition.
			///
			/// \tparam System Must be a class that is move constructable and assignable.
			///
			/// \param name Name of system class in string format.
			///
			template<valid_component System>
			void register_system(const std::string& name);

			///
			/// Create a system using the factory.
			///
			/// \param name Name of system class in string format.
			/// \param storage Storage map to create the system in.
			///
			void create_system(const std::string& name, SystemStack& stack);

		private:
			///
			/// Lets us construct a system class from a string representation.
			///
			ankerl::unordered_dense::map<std::uint64_t, std::move_only_function<void(SystemStack&)>> m_factory;
		};

		template<valid_component System>
		inline void SystemFactory::register_system(const std::string& name)
		{
			const auto hash = math::fnv1a(name.c_str());
			if (!m_factory.contains(hash))
			{
				m_factory.emplace(hash, [](SystemStack& stack) {
					for (auto&& sys : stack)
					{
						if (sys->id() == name)
						{
							GALAXY_LOG(GALAXY_WARNING, "Tried to create a create a duplicate system '{0}'.", name);
							return;
						}
					}

					stack.emplace_back(std::move(std::make_unique<System>()));
				});
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Tried to create a duplicate register system '{0}'.", name);
			}
		}
	} // namespace meta
} // namespace galaxy

#endif
