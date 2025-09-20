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
#include "galaxy/systems/System.hpp"

namespace galaxy
{
	///
	/// System stack typedef.
	///
	using SystemStack = std::vector<std::shared_ptr<System>>;

	///
	/// Meta factory for creating systems.
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
		/// Registers a system into the engine.
		///
		/// \tparam _System Must be a class that is move constructable and assignable.
		///
		/// \param name Name of system class in string format.
		///
		template<meta::valid_component _System>
		void register_system(const std::string& name);

		///
		/// Create a system using the factory.
		///
		/// \param name Name of system class in string format.
		/// \param stack System stack where the systems are created in.
		///
		void create_system(const std::string& name, SystemStack& stack);

	private:
		///
		/// Lets us construct a system class from a string representation.
		///
		ankerl::unordered_dense::map<std::uint64_t, std::move_only_function<void(SystemStack&)>> m_factory;
	};

	template<meta::valid_component _System>
	inline void SystemFactory::register_system(const std::string& name)
	{
		const auto hash = math::fnv1a(name.c_str());
		if (!m_factory.contains(hash))
		{
			m_factory.emplace(hash, [name](SystemStack& stack) {
				for (auto&& sys : stack)
				{
					if (sys->id() == name)
					{
						GALAXY_LOG(GALAXY_WARN, "Tried to create a create a duplicate system '{0}'.", name);
						return;
					}
				}

				auto ptr = std::make_shared<_System>(name);
				stack.push_back(std::static_pointer_cast<System>(ptr));
			});
		}
		else
		{
			GALAXY_LOG(GALAXY_WARN, "Tried to create a duplicate register system '{0}'.", name);
		}
	}
} // namespace galaxy

#endif
