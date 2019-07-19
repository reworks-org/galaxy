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

// TODO: redo iterators to filter out invalid entitys?

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

		void insert(uint element) noexcept;
		
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