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

#include "galaxy/entity/Registry.hpp"

namespace galaxy
{
	///
	/// Represents a system that operates on sets of components.
	///
	class System
	{
	public:
		///
		/// Move constructor.
		///
		System(System&&) noexcept;

		///
		/// Move assignment operator.
		///
		System& operator=(System&&) noexcept;

		///
		/// Destructor.
		///
		virtual ~System() noexcept;

		///
		/// Abstract implementation for updating the system. Use the manager to retreive your components.
		///
		/// \param registry Entity registry to process.
		///
		virtual void update(Registry& registry) = 0;

		///
		/// Get identifier.
		///
		/// \return Const string reference.
		///
		[[nodiscard]]
		const std::string& id() const noexcept;

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

		///
		/// Copy assignment operator.
		///
		System& operator=(const System&) = delete;
		///
		/// Copy constructor.
		///
		System(const System&) = delete;

	protected:
		///
		/// Debug id.
		///
		std::string m_id;
	};

	namespace meta
	{
		///
		/// Concept to ensure a system is actually derived from a System.
		///
		/// \tparam Type Type to test.
		///
		template<typename Type>
		concept is_system = std::derived_from<Type, System>;
	} // namespace meta
} // namespace galaxy

#endif
