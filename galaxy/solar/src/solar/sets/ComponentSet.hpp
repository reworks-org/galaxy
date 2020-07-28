///
/// ComponentSet.hpp
///
/// solar
/// See LICENSE.txt.
///

#ifndef SOLAR_COMPONENTSET_HPP_
#define SOLAR_COMPONENTSET_HPP_

#include <protostar/system/Concepts.hpp>
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
	template<pr::is_class component>
	class ComponentSet final : public EntitySet<Entity>
	{
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
		~ComponentSet() noexcept;

		///
		/// Add a component to an entity.
		///
		/// \param entity Entity to add component to.
		/// \param args Arguments of the components constructor.
		///
		/// \return Component that was just constructed.
		///
		template<typename... _args>
		[[maybe_unused]] component* create(const sr::Entity entity, _args&&... args);

		///
		/// Get an entitys component.
		///
		/// \param entity Entity to retrieve component from.
		///
		/// \return Component belonging to the entity.
		///
		[[nodiscard]] component* get(const sr::Entity entity);

		///
		/// Remove the entity and its assossiated component.
		///
		/// \param entity Entity to remove.
		///
		void remove(const sr::Entity entity) override;

		///
		/// Retrieve internal component array.
		///
		/// \return Const reference to a std::vector.
		///
		[[nodiscard]] const std::vector<component>& get_components() noexcept;

	private:
		///
		/// Component storage for this type.
		/// Kept in sync with dense set of entitys.
		///
		std::vector<component> m_components;
	};

	template<pr::is_class component>
	inline ComponentSet<component>::ComponentSet() noexcept
	    : EntitySet {}
	{
	}

	template<pr::is_class component>
	inline ComponentSet<component>::~ComponentSet() noexcept
	{
		// Make sure everything is cleaned up.
		clear();
		m_components.clear();
	}

	template<pr::is_class component>
	template<typename... Args>
	inline component* ComponentSet<component>::create(const sr::Entity entity, Args&&... args)
	{
		// This works because we are appending the entity to the dense array and
		// the component will be in the same position since the two are synced.
		// So by getting dense[0] we get component[0]
		insert(entity);
		m_components.emplace_back(std::forward<Args>(args)...);

		// Return a reference to the component.
		return &(m_components.back());
	}

	template<pr::is_class component>
	inline component* ComponentSet<component>::get(const sr::Entity entity)
	{
		// Access the index the entity is assosiated with to get the component paired with the entity.
		auto opt = find_index(entity);
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

	template<pr::is_class component>
	inline void ComponentSet<component>::remove(const sr::Entity entity)
	{
		// So if we want to destroy an entity/component, easest method is to move the last entity to the one we are erasing
		// then destroy the duplicate. Called swap-and-pop.
		if (has(entity))
		{
			auto opt = find_index(entity);
			if (opt != std::nullopt)
			{
				m_components[opt.value()] = std::move(m_components.back());
				m_components.pop_back();

				m_dense[m_sparse[entity]]     = m_dense[m_size - 1];
				m_sparse[m_dense[m_size - 1]] = m_sparse[entity];
				--m_size;
			}
			else
			{
				PL_LOG(PL_ERROR, "ComponentSet remove() component found nullopt index: {0}.", entity);
			}
		}
		else
		{
			PL_LOG(PL_WARNING, "Tried to remove an component that does not exist on entity: {0}.", entity);
		}
	}

	template<pr::is_class component>
	inline const std::vector<component>& ComponentSet<component>::get_components() noexcept
	{
		return m_components;
	}
} // namespace sr

#endif