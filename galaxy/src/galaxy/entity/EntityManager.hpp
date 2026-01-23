///
/// EntityManager.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_ENTITY_ENTITYMANAGER_HPP_
#define GALAXY_ENTITY_ENTITYMANAGER_HPP_

#include "galaxy/entity/Registry.hpp"

namespace galaxy
{
	///
	/// \brief Class for making creating and managing entities easier.
	///
	/// Uses an entt::registry internally.
	///
	class EntityManager final
	{
	public:
		///
		/// Constructor.
		///
		EntityManager() noexcept;

		///
		/// Move constructor.
		///
		EntityManager(EntityManager&&) noexcept;

		///
		/// Move assignment operator.
		///
		EntityManager& operator=(EntityManager&&) noexcept;

		///
		/// Destructor.
		///
		~EntityManager() noexcept;

		///
		/// Get entity registry.
		///
		/// \return Reference.
		///
		[[nodiscard]]
		Registry& registry() noexcept;

	private:
		///
		/// Copy constructor.
		///
		EntityManager(const EntityManager&) = delete;

		///
		/// Copy assignment operator.
		///
		EntityManager& operator=(const EntityManager&) = delete;

	private:
		///
		/// Entity data.
		///
		Registry m_registry;
	};
} // namespace galaxy

#endif
