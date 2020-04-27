///
/// DualSparseSet.hpp
///
/// solar
/// See LICENSE.txt.
///

#ifndef SOLAR_DUALSPARSESET_HPP_
#define SOLAR_DUALSPARSESET_HPP_

#include "solar/detail/SparseSet.hpp"

///
/// Core namespace.
///
namespace sr
{
	///
	/// Dual sparse set to store components and systems alongside entitys.
	///
	template<typename Component>
	class DualSparseSet final : public SparseSet<Entity>
	{
		///
		/// Make sure Component is a class or struct.
		///
		static_assert(std::is_class<Component>::value);

		///
		/// Friended to manager to allow direct access to internals that cannot have an interface.
		///
		friend class Manager;

	public:
		///
		/// Constructor.
		///
		DualSparseSet() noexcept;

		///
		/// Argument Constructor.
		///
		/// \param reserve Default reserved amount of entities.
		///
		DualSparseSet(const SR_INTEGER reserve) noexcept;

		///
		/// Destructor
		///
		~DualSparseSet() noexcept override;

		///
		/// Add a component to an entity.
		///
		/// \param entity Entity to add component to.
		/// \param args Arguments of the components constructor.
		///
		/// \return Component that was just constructed.
		///
		template<typename... Args>
		Component* add(const sr::Entity entity, Args&&... args) noexcept;

		///
		/// Get an entitys component.
		///
		/// \param entity Entity to retrieve component from.
		///
		/// \return Component belonging to the entity.
		///
		Component* get(const sr::Entity entity);

		///
		/// Remove the entity and its assossiated component.
		///
		/// \param entity Entity to remove.
		///
		void remove(const sr::Entity entity) override;

	private:
		///
		/// Component storage for this type.
		/// Kept in sync with dense set of entitys.
		///
		std::vector<Component> m_components;
	};

	template<typename Component>
	inline DualSparseSet<Component>::DualSparseSet() noexcept
		:SparseSet()
	{
	}

	template<typename Component>
	inline DualSparseSet<Component>::DualSparseSet(const SR_INTEGER reserve) noexcept
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
	inline DualSparseSet<Component>::~DualSparseSet() noexcept
	{
		// Make sure everything is cleaned up.
		clear();
		m_components.clear();
	}

	template<typename Component>
	template<typename ...Args>
	inline Component* DualSparseSet<Component>::add(const sr::Entity entity, Args&& ...args) noexcept
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
	inline Component* DualSparseSet<Component>::get(const sr::Entity entity)
	{
		// Access the index the entity is assosiated with to get the component paired with the entity.
		return &(m_components[findIndex(entity)]);
	}

	template<typename Component>
	inline void DualSparseSet<Component>::remove(const sr::Entity entity)
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