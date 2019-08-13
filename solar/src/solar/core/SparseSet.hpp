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

namespace sr
{
	///
	/// Fast storage of unsigned integers.
	/// Thanks to: https://www.computist.xyz/2018/06/sparse-sets.html
	///
	template<typename uint>
	class SparseSet
	{
		// Make sure its an unsigned integer.
		static_assert(std::is_unsigned<uint>::value, "SparseSet must be an unsigned integer!");

	public:
		///
		/// Iterator using std::vector iterator.
		///
		using Iterator = typename std::vector<uint>::const_iterator;

		///
		/// Const Iterator using std::vector iterator.
		///
		using ConstIterator = typename std::vector<uint>::const_iterator;

		///
		/// Constructor.
		///
		SparseSet() noexcept;

		///
		/// Constructor.
		///
		/// \param reserve Reserve an amount of entities at construction time.
		///
		SparseSet(uint reserve) noexcept;

		///
		/// Destructor.
		///
		virtual ~SparseSet() noexcept;

		///
		/// Insert an element into the sparse set.
		///
		/// \param element Element to insert.
		///
		void insert(uint element) noexcept;
		
		///
		/// Does the sparse set contain an element.
		///
		/// \param element Element to check if exists.
		///
		/// \return boolean value. Returns true if sparse set contains element.
		///
		bool has(uint element) noexcept;

		///
		/// Find the index in the dense array of the element.
		///
		/// \param element Entity to find the index of.
		///
		/// \return unsigned integer - the index of the element.
		///
		uint findIndex(uint element);

		///
		/// Reserve an amount of space for new entities.
		///
		/// \param newReserve Value to reserve in the sparse set.
		///
		void reserve(uint newReserve) noexcept;
		
		///
		/// Removes an entity. Usually overriden by ExtendedSet to
		/// destroy components aswell.
		///
		/// \param element Element to remove from the sparse set.
		///
		virtual void remove(uint element);
		
		///
		/// Check if sparse set is entity.
		///
		/// \return boolean True if empty.
		///
		bool empty() noexcept;
		
		///
		/// Destroy all elements and clear the sparse set.
		///
		void clear() noexcept;

		///
		/// Get the size of the sparse set.
		///
		/// \return Unsigned Integer.
		///
		uint size() const noexcept;

		///
		/// Get the capacity of the sparse set.
		///
		/// \return Unsigned Integer.
		///
		uint capacity() const noexcept;

		///
		/// Iterator start.
		///
		/// \return Beginning iterator.
		///
		inline Iterator begin()
		{
			return m_dense.begin();
		}

		///
		/// Const Iterator start.
		///
		/// \return Const beginning iterator.
		///
		inline ConstIterator begin() const
		{
			return m_dense.begin();
		}

		///
		/// Iterator end.
		///
		/// \return Ending iterator.
		///
		inline Iterator end()
		{
			return m_dense.begin() + m_size;
		}

		///
		/// Const Iterator end.
		///
		/// \return Const ending iterator.
		///
		inline ConstIterator end() const
		{
			return m_dense.begin() + m_size;
		}

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
	inline SparseSet<uint>::SparseSet(uint newReserve) noexcept
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
	inline void SparseSet<uint>::insert(uint element) noexcept
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
	inline bool SparseSet<uint>::has(uint element) noexcept
	{
		return element < m_capacity && m_sparse[element] < m_size && m_dense[m_sparse[element]] == element;
	}

	template<typename uint>
	inline uint SparseSet<uint>::findIndex(uint element)
	{
		if (element > m_sparse.size())
		{
			std::string msg = "Out of bounds! Sparse Set does not contain element: " + std::to_string(element);
			throw std::out_of_range(msg);
		}
		
		return m_sparse[element];
	}

	template<typename uint>
	inline void SparseSet<uint>::reserve(uint reserve) noexcept
	{
		if (reserve > m_capacity)
		{
			m_dense.resize(reserve, 999);
			m_sparse.resize(reserve, 999);

			m_capacity = reserve;
		}
	}

	template<typename uint>
	inline void SparseSet<uint>::remove(uint element)
	{
		if (has(element))
		{
			m_dense[m_sparse[element]] = m_dense[m_size - 1];
			m_sparse[m_dense[m_size - 1]] = m_sparse[element];
			--m_size;
		}
	}

	template<typename uint>
	inline bool SparseSet<uint>::empty() noexcept
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
	inline uint SparseSet<uint>::size() const noexcept
	{
		return m_size;
	}

	template<typename uint>
	inline uint SparseSet<uint>::capacity() const noexcept
	{
		return m_capacity;
	}
}

#endif