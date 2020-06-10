///
/// ComponentSet.hpp
///
/// solar
/// See LICENSE.txt.
///

#ifndef SOLAR_COMPONENTSET_HPP_
#define SOLAR_COMPONENTSET_HPP_

#include <pulsar/Log.hpp>

#include "solar/sets/EntitySet.hpp"

///
/// Core namespace.
///
namespace sr
{
	///
	/// Dual sparse set to store components and systems alongside entitys.
	///
	template<typename Component>
	class ComponentSet final : public EntitySet<Entity>
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
		ComponentSet() noexcept;

		///
		/// Destructor
		///
		~ComponentSet() noexcept override;

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
		Component* get(const sr::Entity entity) noexcept;

		///
		/// Remove the entity and its assossiated component.
		///
		/// \param entity Entity to remove.
		///
		void remove(const sr::Entity entity) noexcept override;

	private:
		///
		/// Component storage for this type.
		/// Kept in sync with dense set of entitys.
		///
		std::vector<Component> m_components;
	};

	template<typename Component>
	inline ComponentSet<Component>::ComponentSet() noexcept
		:EntitySet()
	{
	}

	template<typename Component>
	inline ComponentSet<Component>::~ComponentSet() noexcept
	{
		// Make sure everything is cleaned up.
		clear();
		m_components.clear();
	}

	template<typename Component>
	template<typename ...Args>
	inline Component* ComponentSet<Component>::add(const sr::Entity entity, Args&& ...args) noexcept
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
	inline Component* ComponentSet<Component>::get(const sr::Entity entity) noexcept
	{
		// Access the index the entity is assosiated with to get the component paired with the entity.
		auto opt = findIndex(entity);
		if (opt != std::nullopt)
		{
			return &m_components[opt.value()];
		}
		else
		{
			PL_LOG(PL_FATAL, "ComponentSet get() component found nullopt index.");
			return nullptr;
		}
	}

	template<typename Component>
	inline void ComponentSet<Component>::remove(const sr::Entity entity) noexcept
	{
		// So if we want to destroy an entity/component, easest method is to move the last entity to the one we are erasing
		// then destroy the duplicate. Called swap-and-pop.
		if (has(entity))
		{
			auto opt = findIndex(entity);
			if (opt != std::nullopt)
			{
				m_components[opt.value()] = std::move(m_components.back());
				m_components.pop_back();

				m_dense[m_sparse[entity]] = m_dense[m_size - 1];
				m_sparse[m_dense[m_size - 1]] = m_sparse[entity];
				--m_size;
			}
			else
			{
				PL_LOG(PL_ERROR, "ComponentSet remove() component found nullopt index: " + std::to_string(entity));
			}
		}
		else
		{
			PL_LOG(PL_WARNING, "Tried to remove an component that does not exist on entity: " + std::to_string(entity));
		}
	}
}

#endif