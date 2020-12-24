///
/// EntitySet.hpp
/// solar
///
/// See LICENSE.txt.
///

#ifndef SOLAR_ENTITYSET_HPP_
#define SOLAR_ENTITYSET_HPP_

#include <concepts>
#include <optional>
#include <vector>

#include "galaxy/error/Log.hpp"

#include "solar/Config.hpp"

///
/// Core namespace.
///
namespace sr
{
	///
	/// Ensures the entity type is an insigned integer.
	///
	template<typename Type>
	concept is_unsigned_integer = (std::is_unsigned<Type>::value && std::is_integral<Type>::value);

	///
	/// Fast storage of unsigned integers.
	///
	template<is_unsigned_integer UInt>
	class EntitySet
	{
		///
		/// Friended to manager to allow direct access to internals that cannot have an interface.
		///
		friend class Manager;

	public:
		///
		/// Constructor.
		///
		EntitySet();

		///
		/// Copy constructor.
		///
		EntitySet(const EntitySet&) = delete;

		///
		/// Move constructor.
		///
		EntitySet(EntitySet&&) = delete;

		///
		/// Copy assignment operator.
		///
		EntitySet& operator=(const EntitySet&) = delete;

		///
		/// Move assignment operator.
		///
		EntitySet& operator=(EntitySet&&) = delete;

		///
		/// Destructor.
		///
		virtual ~EntitySet();

		///
		/// Insert an element into the Entity Set.
		///
		/// \param element Element to insert.
		///
		void insert(const UInt element);

		///
		/// Does the Entity Set contain an element.
		///
		/// \param element Element to check if exists.
		///
		/// \return Boolean value. Returns true if Entity Set contains element.
		///
		[[nodiscard]] bool has(const UInt element);

		///
		/// Find the index in the dense array of the element.
		///
		/// \param element Entity to find the index of.
		///
		/// \return The index of the element. Check for validity against std::nullopt.
		///
		[[nodiscard]] std::optional<UInt> find_index(const UInt element);

		///
		/// Removes an entity. Usually overriden by DualEntitySet to
		/// destroy components aswell.
		///
		/// \param element Element to remove from the Entity Set.
		///
		virtual void remove(const UInt element);

		///
		/// Destroy all elements and clear the Entity Set.
		///
		void clear();

		///
		/// Check if Entity Set is entity.
		///
		/// \return Boolean true if empty.
		///
		[[nodiscard]] const bool empty();

		///
		/// Get the size of the Entity Set.
		///
		/// \return Unsigned Integer.
		///
		[[nodiscard]] const UInt size() const;

		///
		/// Get the capacity of the Entity Set.
		///
		/// \return Unsigned Integer.
		///
		[[nodiscard]] const UInt capacity() const;

		///
		/// Iterator start.
		///
		/// \return Beginning iterator.
		///
		[[nodiscard]] decltype(auto) begin();

		///
		/// Const Iterator start.
		///
		/// \return Const beginning iterator.
		///
		[[nodiscard]] decltype(auto) begin() const;

		///
		/// Iterator end.
		///
		/// \return Ending iterator.
		///
		[[nodiscard]] decltype(auto) end();

		///
		/// Const Iterator end.
		///
		/// \return Const ending iterator.
		///
		[[nodiscard]] decltype(auto) end() const;

	protected:
		///
		/// Current number of elements in Entity Set.
		///
		UInt m_size;

		///
		/// Total avaliable space for elements.
		/// Formula is: total_inserts + 1.
		///
		UInt m_capacity;

		///
		/// The actual elements are stored here.
		///
		std::vector<UInt> m_dense;

		///
		/// The elements are used as an index. They are indexes of the dense array.
		///
		std::vector<UInt> m_sparse;
	};

	template<is_unsigned_integer UInt>
	inline EntitySet<UInt>::EntitySet()
	    : m_size {0}, m_capacity {0}
	{
	}

	template<is_unsigned_integer UInt>
	inline EntitySet<UInt>::~EntitySet()
	{
		clear();
	}

	template<is_unsigned_integer UInt>
	inline void EntitySet<UInt>::insert(const UInt element)
	{
		if (!has(element))
		{
			if (element >= m_capacity)
			{
				const UInt new_size = element + static_cast<UInt>(1);
				m_dense.resize(new_size);
				m_sparse.resize(new_size);
				m_capacity = new_size;
			}

			m_dense[m_size]   = element;
			m_sparse[element] = m_size;
			m_size++;
		}
		else
		{
			GALAXY_LOG(GALAXY_ERROR, "Attempted to insert duplicate element {0}.", element);
		}
	}

	template<is_unsigned_integer UInt>
	inline bool EntitySet<UInt>::has(const UInt element)
	{
		return element < m_capacity && m_sparse[element] < m_size && m_dense[m_sparse[element]] == element;
	}

	template<is_unsigned_integer UInt>
	inline std::optional<UInt> EntitySet<UInt>::find_index(const UInt element)
	{
		if (element >= m_sparse.size() || m_sparse.empty())
		{
			GALAXY_LOG(GALAXY_FATAL, "Out of bounds! Entity Set does not contain element: {0}.", element);
			return std::nullopt;
		}
		else
		{
			return std::make_optional(m_sparse[element]);
		}
	}

	template<is_unsigned_integer UInt>
	inline void EntitySet<UInt>::remove(const UInt element)
	{
		if (has(element))
		{
			m_dense[m_sparse[element]]    = m_dense[m_size - 1];
			m_sparse[m_dense[m_size - 1]] = m_sparse[element];
			--m_size;
		}
	}

	template<is_unsigned_integer UInt>
	inline void EntitySet<UInt>::clear()
	{
		m_dense.clear();
		m_sparse.clear();

		m_capacity = 0;
		m_size     = 0;
	}

	template<is_unsigned_integer UInt>
	inline const bool EntitySet<UInt>::empty()
	{
		return m_size == 0;
	}

	template<is_unsigned_integer UInt>
	inline const UInt EntitySet<UInt>::size() const
	{
		return m_size;
	}

	template<is_unsigned_integer UInt>
	inline const UInt EntitySet<UInt>::capacity() const
	{
		return m_capacity;
	}

	template<is_unsigned_integer UInt>
	inline decltype(auto) EntitySet<UInt>::begin()
	{
		return m_dense.begin();
	}

	template<is_unsigned_integer UInt>
	inline decltype(auto) EntitySet<UInt>::begin() const
	{
		return m_dense.begin();
	}

	template<is_unsigned_integer UInt>
	inline decltype(auto) EntitySet<UInt>::end()
	{
		return m_dense.begin() + m_size;
	}

	template<is_unsigned_integer UInt>
	inline decltype(auto) EntitySet<UInt>::end() const
	{
		return m_dense.begin() + m_size;
	}
} // namespace sr

#endif