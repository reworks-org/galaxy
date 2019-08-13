///
/// ExtendedSet.hpp
///
/// solar
/// See LICENSE.txt.
///

#ifndef SOLAR_EXTENDEDSET_HPP_
#define SOLAR_EXTENDEDSET_HPP_

#include "solar/core/SparseSet.hpp"

namespace sr
{
	///
	/// Extension of sparse set to store components and systems.
	/// Thanks to: https://skypjack.github.io/2019-03-21-ecs-baf-part-2-insights/
	///
	template<typename Component>
	class ExtendedSet final : public SparseSet<Entity>
	{
		///
		/// Friended to manager to allow direct access to internals that cannot have an interface.
		///
		friend class Manager;
	public:
		///
		/// Constructor.
		///
		ExtendedSet() noexcept;

		///
		/// Value Constructor.
		///
		/// \param reserve Default reserved amount of entities.
		///
		ExtendedSet(SR_INTEGER reserve) noexcept;

		///
		/// Destructor
		///
		~ExtendedSet() noexcept override;

		///
		/// Add a component to an entity.
		///
		/// \param entity Entity to add component to.
		/// \param args Arguments of the components constructor.
		///
		/// \return Component that was just constructed.
		///
		template<typename... Args>
		Component* add(Entity entity, Args&&... args);

		///
		/// Get an entitys component.
		///
		/// \param entity Entity to retrieve component from.
		///
		/// \return Component belonging to the entity.
		///
		Component* get(Entity entity);

		///
		/// Remove the entity and its assossiated component.
		///
		/// \param entity Entity to remove.
		///
		void remove(Entity entity) override;

	private:
		///
		/// Component storage for this type.
		/// Kept in sync with dense set of entitys.
		///
		std::vector<Component> m_components;
	};

	template<typename Component>
	inline ExtendedSet<Component>::ExtendedSet() noexcept
		:SparseSet()
	{
	}

	template<typename Component>
	inline ExtendedSet<Component>::ExtendedSet(SR_INTEGER reserve) noexcept
		:SparseSet(reserve)
	{
		// Make sure minimum reserve size is 1.
		if (reserve < 1)
		{
			m_components.resize(1);
		}
		else
		{
			m_components.resize(reserve);
		}
	}

	template<typename Component>
	inline ExtendedSet<Component>::~ExtendedSet() noexcept
	{
		// Make sure everything is cleaned up.
		clear();
		m_components.clear();
	}

	template<typename Component>
	template<typename ...Args>
	inline Component* ExtendedSet<Component>::add(Entity entity, Args&& ...args)
	{
		// This works because we are appending the entity to the dense array and
		// the component will be in the same position since the two are synced.
		// So by getting dense[0] we get component[0]
		insert(entity);
		m_components.emplace_back(std::forward<Args>(args)...);

		// Return a reference to the component.
		return &(m_components.back());
	}

	template<typename Component>
	inline Component* ExtendedSet<Component>::get(Entity entity)
	{
		// Access the index the entity is assosiated with to get the component paired with the entity.
		return &(m_components[findIndex(entity)]);
	}

	template<typename Component>
	inline void ExtendedSet<Component>::remove(Entity entity)
	{
		// So if we want to destroy an entity/component, easest method is to move the last entity to the one we are erasing
		// then destroy the duplicate. Called swap-and-pop.
		if (has(entity))
		{
			m_components[findIndex(entity)] = std::move(m_components.back());
			m_components.pop_back();

			m_dense[m_sparse[entity]] = m_dense[m_size - 1];
			m_sparse[m_dense[m_size - 1]] = m_sparse[entity];
			--m_size;
		}
	}
}

#endif