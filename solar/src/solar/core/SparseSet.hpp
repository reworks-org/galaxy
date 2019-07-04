///
/// SparseSet.hpp
///
/// solar
/// See LICENSE.txt.
///

#ifndef SOLAR_SPARSESET_HPP_
#define SOLAR_SPARSESET_HPP_

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
		using Iterator = typename std::vector<uint>::const_iterator;
		using ConstIterator = typename std::vector<uint>::const_iterator;

		SparseSet() noexcept;

		SparseSet(uint reserve) noexcept;

		virtual ~SparseSet() noexcept;

		uint insert(uint element) noexcept;
		
		bool has(uint element) noexcept;

		///
		/// Find the index in the dense array of the element.
		///
		uint findIndex(uint element);

		void reserve(uint newReserve) noexcept;
		
		///
		/// Removes an entity. Usually overriden by ExtendedSet to
		/// destroy components aswell.
		///
		virtual void remove(uint element);
			
		bool empty() noexcept;
		
		void clear() noexcept;

		uint size() const noexcept;

		uint capacity() const noexcept;

		inline Iterator begin()
		{
			return m_dense.begin();
		}

		inline ConstIterator begin() const
		{
			return m_dense.begin();
		}

		inline Iterator end()
		{
			return m_dense.begin() + m_size;
		}

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
		clear();
	}

	template<typename uint>
	inline uint SparseSet<uint>::insert(uint element) noexcept
	{
		if (element >= m_capacity)
		{
			reserve(element + 1);
		}

		m_dense[m_size] = element;
		m_sparse[element] = m_size;
		++m_size;

		return m_dense[m_sparse[element]];
	}

	template<typename uint>
	inline bool SparseSet<uint>::has(uint element) noexcept
	{
		bool success = false;

		if (element < m_capacity)
		{
			if (m_sparse[element] < m_size)
			{
				if (m_dense[m_sparse[element]] == element)
				{
					success = true;
				}
			}
		}

		return success;
	}

	template<typename uint>
	inline uint SparseSet<uint>::findIndex(uint element)
	{
		if (!has(element))
		{
			throw std::out_of_range("Out of bounds! Does not contain element.");
		}
		
		return m_sparse[element];
	}

	template<typename uint>
	inline void SparseSet<uint>::reserve(uint reserve) noexcept
	{
		if (reserve > m_capacity)
		{
			m_dense.resize(reserve, 0);
			m_sparse.resize(reserve, 0);

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