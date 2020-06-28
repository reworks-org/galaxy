///
/// EntitySet.hpp
///
/// solar
/// See LICENSE.txt.
///

#ifndef SOLAR_ENTITYSET_HPP_
#define SOLAR_ENTITYSET_HPP_

#include <string>
#include <vector>
#include <optional>

#include <pulsar/Log.hpp>

#include "solar/Config.hpp"

///
/// Core namespace.
///
namespace sr
{
	///
	/// Fast storage of unsigned integers.
	///
	template<typename uint>
	class EntitySet
	{
		// Make sure its an unsigned integer.
		static_assert(std::is_unsigned<uint>::value, "EntitySet must be an unsigned integer!");

	public:
		///
		/// Constructor.
		///
		EntitySet() noexcept;

		///
		/// Destructor.
		///
		virtual ~EntitySet() noexcept;

		///
		/// Insert an element into the Entity Set.
		///
		/// \param element Element to insert.
		///
		void insert(const uint element) noexcept;
		
		///
		/// Does the Entity Set contain an element.
		///
		/// \param element Element to check if exists.
		///
		/// \return Boolean value. Returns true if Entity Set contains element.
		///
		bool has(const uint element) noexcept;

		///
		/// Find the index in the dense array of the element.
		///
		/// \param element Entity to find the index of.
		///
		/// \return The index of the element. Check for validity against std::nullopt.
		///
		std::optional<uint> findIndex(const uint element) noexcept;
		
		///
		/// Removes an entity. Usually overriden by DualEntitySet to
		/// destroy components aswell.
		///
		/// \param element Element to remove from the Entity Set.
		///
		virtual void remove(const uint element) noexcept;
		
		///
		/// Check if Entity Set is entity.
		///
		/// \return Boolean true if empty.
		///
		const bool empty() noexcept;
		
		///
		/// Destroy all elements and clear the Entity Set.
		///
		void clear() noexcept;

		///
		/// Get the size of the Entity Set.
		///
		/// \return Unsigned Integer.
		///
		const uint size() const noexcept;

		///
		/// Get the capacity of the Entity Set.
		///
		/// \return Unsigned Integer.
		///
		const uint capacity() const noexcept;

		///
		/// Iterator start.
		///
		/// \return Beginning iterator.
		///
		decltype(auto) begin() noexcept;

		///
		/// Const Iterator start.
		///
		/// \return Const beginning iterator.
		///
		decltype(auto) begin() const noexcept;

		///
		/// Iterator end.
		///
		/// \return Ending iterator.
		///
		decltype(auto) end() noexcept;

		///
		/// Const Iterator end.
		///
		/// \return Const ending iterator.
		///
		decltype(auto) end() const noexcept;

	protected:
		///
		/// Current number of elements in Entity Set.
		///
		uint m_size;

		///
		/// Total avaliable space for elements.
		/// Formula is: total_inserts + 1.
		///
		uint m_capacity;

		///
		/// The actual elements are stored here.
		///
		std::vector<uint> m_dense;

		///
		/// The elements are used as an index. They are indexes of the dense array.
		///
		std::vector<uint> m_sparse;
	};

	template<typename uint>
	inline EntitySet<uint>::EntitySet() noexcept
		:m_size(0), m_capacity(0)
	{
	}

	template<typename uint>
	inline EntitySet<uint>::~EntitySet() noexcept
	{
		// Clean up.
		clear();
	}

	template<typename uint>
	inline void EntitySet<uint>::insert(const uint element) noexcept
	{
		if (!has(element))
		{
			if (element >= m_capacity)
			{
				m_dense.resize(element + 1, std::numeric_limits<uint>::max);
				m_sparse.resize(element + 1, std::numeric_limits<uint>::max);
				m_capacity = element + 1;
			}

			m_dense[m_size] = element;
			m_sparse[element] = m_size;
			m_size++;
		}
		else
		{
			PL_LOG(PL_ERROR, "Attempted to insert duplicate element.");
		}
	}

	template<typename uint>
	inline bool EntitySet<uint>::has(const uint element) noexcept
	{
		return element < m_capacity && m_sparse[element] < m_size && m_dense[m_sparse[element]] == element;
	}

	template<typename uint>
	inline std::optional<uint> EntitySet<uint>::findIndex(const uint element) noexcept
	{
		if (element >= m_sparse.size() || m_sparse.empty())
		{
			PL_LOG(PL_FATAL, "Out of bounds! Entity Set does not contain element: " + std::to_string(element));
			return std::nullopt;
		}
		else
		{
			return std::make_optional(m_sparse[element]);
		}
	}

	template<typename uint>
	inline void EntitySet<uint>::remove(const uint element) noexcept
	{
		if (has(element))
		{
			m_dense[m_sparse[element]] = m_dense[m_size - 1];
			m_sparse[m_dense[m_size - 1]] = m_sparse[element];
			--m_size;
		}
	}

	template<typename uint>
	inline const bool EntitySet<uint>::empty() noexcept
	{
		return m_size == 0;
	}

	template<typename uint>
	inline void EntitySet<uint>::clear() noexcept
	{
		m_dense.clear();
		m_sparse.clear();

		m_capacity = 0;
		m_size = 0;
	}

	template<typename uint>
	inline const uint EntitySet<uint>::size() const noexcept
	{
		return m_size;
	}

	template<typename uint>
	inline const uint EntitySet<uint>::capacity() const noexcept
	{
		return m_capacity;
	}

	template<typename uint>
	inline decltype(auto) EntitySet<uint>::begin() noexcept
	{
		return m_dense.begin();
	}

	template<typename uint>
	inline decltype(auto) EntitySet<uint>::begin() const noexcept
	{
		return m_dense.begin();
	}

	template<typename uint>
	inline decltype(auto) EntitySet<uint>::end() noexcept
	{
		return m_dense.begin() + m_size;
	}

	template<typename uint>
	inline decltype(auto) EntitySet<uint>::end() const noexcept
	{
		return m_dense.begin() + m_size;
	}
}

#endif