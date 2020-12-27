///
/// Set.hpp
/// galaxy
///
/// See LICENSE.txt.
///

#ifndef GALAXY_ECS_SET_HPP_
#define GALAXY_ECS_SET_HPP_

#include <vector>

#include <robin_hood.h>

#include "galaxy/ecs/Entity.hpp"

namespace galaxy
{
	namespace core
	{
		class World;
	} // namespace core

	namespace ecs
	{
		///
		/// A set of entities within an ECS.
		///
		class Set
		{
			friend class core::World;

		public:
			///
			/// Virtual destructor.
			///
			virtual ~Set() = default;

			///
			/// Check if set already has entity.
			///
			/// \param entity Entity to check.
			///
			/// \return Const boolean. True if entity is found.
			///
			const bool has(const Entity entity);

			///
			/// Remove the entity and its assossiated component.
			///
			/// \param entity Entity to remove.
			///
			virtual void remove(const Entity entity) = 0;

			///
			/// Erase all data in set.
			///
			virtual void clear() = 0;

		protected:
			///
			/// Constructor.
			///
			Set();

			///
			/// Copy constructor.
			///
			Set(const Set&) = delete;

			///
			/// Move constructor.
			///
			Set(Set&&) = delete;

			///
			/// Copy assignment operator.
			///
			Set& operator=(const Set&) = delete;

			///
			/// Move assignment operator.
			///
			Set& operator=(Set&&) = delete;

			///
			/// Keeps track of entity positions in vector.
			///
			std::size_t m_count;

			///
			/// Entitys that have this component.
			///
			std::vector<Entity> m_entities;

			///
			/// Keeps track of entity position using entity as key.
			///
			robin_hood::unordered_flat_map<Entity, std::size_t> m_keymap;
		};
	} // namespace ecs
} // namespace galaxy

#endif