///
/// ComponentSet.hpp
/// galaxy
///
/// See LICENSE.txt.
///

#ifndef GALAXY_ECS_COMPONENTSET_HPP_
#define GALAXY_ECS_COMPONENTSET_HPP_

#include "galaxy/error/Log.hpp"
#include "galaxy/ecs/Set.hpp"
#include "galaxy/meta/Concepts.hpp"

namespace galaxy
{
	namespace core
	{
		class World;
	} // namespace core

	namespace ecs
	{
		///
		/// Dual sparse set to store components and systems alongside entitys.
		///
		template<meta::is_class Component>
		class ComponentSet final : public Set
		{
			friend class core::World;

		public:
			///
			/// Constructor.
			///
			ComponentSet() noexcept;

			///
			/// Virtual destructor
			///
			virtual ~ComponentSet();

			///
			/// Add a component to an entity.
			///
			/// \param entity Entity to add component to.
			/// \param args Arguments of the components constructor.
			///
			/// \return Component that was just constructed.
			///
			template<typename... Args>
			[[maybe_unused]] Component* create(const Entity entity, Args&&... args);

			///
			/// Get an entitys component.
			///
			/// \param entity Entity to retrieve component from.
			///
			/// \return Component belonging to the entity.
			///
			[[nodiscard]] Component* get(const Entity entity);

			///
			/// Remove the entity and its assossiated component.
			///
			/// \param entity Entity to remove.
			///
			virtual void remove(const Entity entity) override;

			///
			/// Erase all data in set.
			///
			virtual void clear() override;

			///
			/// Get size of set.
			///
			/// \return Const unsigned int.
			///
			[[nodiscard]] const unsigned int get_size() const noexcept override;

			///
			/// Retrieve internal component array.
			///
			/// \return Const reference to a std::vector.
			///
			[[nodiscard]] const std::vector<Component>& get_components() noexcept;

		private:
			///
			/// Copy constructor.
			///
			ComponentSet(const ComponentSet&) = delete;

			///
			/// Move constructor.
			///
			ComponentSet(ComponentSet&&) = delete;

			///
			/// Copy assignment operator.
			///
			ComponentSet& operator=(const ComponentSet&) = delete;

			///
			/// Move assignment operator.
			///
			ComponentSet& operator=(ComponentSet&&) = delete;

		private:
			///
			/// Component storage for this type.
			///
			std::vector<Component> m_components;
		};

		template<meta::is_class Component>
		inline ComponentSet<Component>::ComponentSet() noexcept
			: Set {}
		{
		}

		template<meta::is_class Component>
		inline ComponentSet<Component>::~ComponentSet()
		{
			clear();
		}

		template<meta::is_class Component>
		template<typename... Args>
		inline Component* ComponentSet<Component>::create(const Entity entity, Args&&... args)
		{
			m_entities.push_back(entity);
			m_components.emplace_back(std::forward<Args>(args)...);

			m_keymap[entity] = m_count++;

			return &m_components.back();
		}

		template<meta::is_class Component>
		inline Component* ComponentSet<Component>::get(const Entity entity)
		{
			if (m_keymap.contains(entity))
			{
				return &m_components[m_keymap[entity]];
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to find a component from provided entity to get.");
				return nullptr;
			}
		}

		template<meta::is_class Component>
		inline void ComponentSet<Component>::remove(const Entity entity)
		{
			if (m_keymap.contains(entity))
			{
				const auto end = m_components.size() - 1;

				auto temp_comp = std::move(m_components[end]);
				auto temp_ent  = m_entities[end];

				m_components[end] = std::move(m_components[m_keymap[entity]]);
				m_entities[end]   = m_entities[m_keymap[entity]];

				m_components[m_keymap[entity]] = std::move(temp_comp);
				m_entities[m_keymap[entity]]   = temp_ent;
				m_keymap[temp_ent]             = m_keymap[entity];

				m_components.pop_back();
				m_entities.pop_back();
				m_keymap.erase(entity);
				m_count--;
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to find a component from provided entity to remove.");
			}
		}

		template<meta::is_class Component>
		inline void ComponentSet<Component>::clear()
		{
			m_components.clear();
			m_entities.clear();
			m_keymap.clear();

			m_count = 0;
		}

		template<meta::is_class Component>
		inline const unsigned int ComponentSet<Component>::get_size() const noexcept
		{
			return static_cast<unsigned int>(m_components.size());
		}

		template<meta::is_class Component>
		inline const std::vector<Component>& ComponentSet<Component>::get_components() noexcept
		{
			return m_components;
		}
	} // namespace ecs
} // namespace galaxy

#endif