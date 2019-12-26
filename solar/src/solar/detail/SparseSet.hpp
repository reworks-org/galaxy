///
/// SparseSet.hpp
///
/// solar
/// See LICENSE.txt.
///

#ifndef SOLAR_SPARSESET_HPP_
#define SOLAR_SPARSESET_HPP_

#include <string>
#include <vector>

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
	class SparseSet
	{
		// Make sure its an unsigned integer.
		static_assert(std::is_unsigned<uint>::value, "SparseSet must be an unsigned integer!");

	public:
		///
		/// Constructor.
		///
		SparseSet() noexcept;

		///
		/// Constructor.
		///
		/// \param reserve Reserve an amount of entities at construction time.
		///
		SparseSet(const uint reserve) noexcept;

		///
		/// Destructor.
		///
		virtual ~SparseSet() noexcept;

		///
		/// Insert an element into the sparse set.
		///
		/// \param element Element to insert.
		///
		void insert(const uint element) noexcept;
		
		///
		/// Does the sparse set contain an element.
		///
		/// \param element Element to check if exists.
		///
		/// \return Boolean value. Returns true if sparse set contains element.
		///
		bool has(const uint element) noexcept;

		///
		/// Find the index in the dense array of the element.
		///
		/// \param element Entity to find the index of.
		///
		/// \return Unsigned integer - the index of the element.
		///
		uint findIndex(const uint element);

		///
		/// Reserve an amount of space for new entities.
		///
		/// \param newReserve Value to reserve in the sparse set.
		///
		void reserve(const uint newReserve) noexcept;
		
		///
		/// Removes an entity. Usually overriden by DualSparseSet to
		/// destroy components aswell.
		///
		/// \param element Element to remove from the sparse set.
		///
		virtual void remove(const uint element);
		
		///
		/// Check if sparse set is entity.
		///
		/// \return Boolean true if empty.
		///
		const bool empty() noexcept;
		
		///
		/// Destroy all elements and clear the sparse set.
		///
		void clear() noexcept;

		///
		/// Get the size of the sparse set.
		///
		/// \return Unsigned Integer.
		///
		const uint size() const noexcept;

		///
		/// Get the capacity of the sparse set.
		///
		/// \return Unsigned Integer.
		///
		const uint capacity() const noexcept;

		///
		/// Iterator start.
		///
		/// \return Beginning iterator.
		///
		decltype(auto) begin();

		///
		/// Const Iterator start.
		///
		/// \return Const beginning iterator.
		///
		decltype(auto) begin() const;

		///
		/// Iterator end.
		///
		/// \return Ending iterator.
		///
		decltype(auto) end();

		///
		/// Const Iterator end.
		///
		/// \return Const ending iterator.
		///
		decltype(auto) end() const;

	protected:
		///
		/// Current number of elements in sparse set.
		///
		uint m_size;

		///
		/// Capacity (max value + 1).
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
	inline SparseSet<uint>::SparseSet() noexcept
		:m_size(0), m_capacity(0)
	{
	}

	template<typename uint>
	inline SparseSet<uint>::SparseSet(const uint newReserve) noexcept
		:m_size(0), m_capacity(0)
	{
		reserve(newReserve);
	}

	template<typename uint>
	inline SparseSet<uint>::~SparseSet() noexcept
	{
		// Clean up.
		clear();
	}

	template<typename uint>
	inline void SparseSet<uint>::insert(const uint element) noexcept
	{
		if (!has(element))
		{
			if (element >= m_capacity)
			{
				reserve(element + 1);
			}

			m_dense[m_size] = element;
			m_sparse[element] = m_size;
			++m_size;
		}
	}

	template<typename uint>
	inline bool SparseSet<uint>::has(const uint element) noexcept
	{
		return element < m_capacity && m_sparse[element] < m_size && m_dense[m_sparse[element]] == element;
	}

	template<typename uint>
	inline uint SparseSet<uint>::findIndex(const uint element)
	{
		if (element > m_sparse.size())
		{
			std::string msg = "Out of bounds! Sparse Set does not contain element: " + std::to_string(element);
			throw std::out_of_range(msg);
		}
		
		return m_sparse[element];
	}

	template<typename uint>
	inline void SparseSet<uint>::reserve(const uint reserve) noexcept
	{
		if (reserve > m_capacity)
		{
			m_dense.resize(reserve, 999);
			m_sparse.resize(reserve, 999);

			m_capacity = reserve;
		}
	}

	template<typename uint>
	inline void SparseSet<uint>::remove(const uint element)
	{
		if (has(element))
		{
			m_dense[m_sparse[element]] = m_dense[m_size - 1];
			m_sparse[m_dense[m_size - 1]] = m_sparse[element];
			--m_size;
		}
	}

	template<typename uint>
	inline const bool SparseSet<uint>::empty() noexcept
	{
		return m_size == 0;
	}

	template<typename uint>
	inline void SparseSet<uint>::clear() noexcept
	{
		m_dense.clear();
		m_sparse.clear();

		m_capacity = 0;
		m_size = 0;
	}

	template<typename uint>
	inline const uint SparseSet<uint>::size() const noexcept
	{
		return m_size;
	}

	template<typename uint>
	inline const uint SparseSet<uint>::capacity() const noexcept
	{
		return m_capacity;
	}

	template<typename uint>
	inline decltype(auto) SparseSet<uint>::begin()
	{
		return m_dense.begin();
	}

	template<typename uint>
	inline decltype(auto) SparseSet<uint>::begin() const
	{
		return m_dense.begin();
	}

	template<typename uint>
	inline decltype(auto) SparseSet<uint>::end()
	{
		return m_dense.begin() + m_size;
	}

	template<typename uint>
	inline decltype(auto) SparseSet<uint>::end() const
	{
		return m_dense.begin() + m_size;
	}
}

#endif