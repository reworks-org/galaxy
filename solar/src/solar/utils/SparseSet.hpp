///
/// SparseSet.hpp
///
/// solar
/// See LICENSE.txt.
///

#ifndef SOLAR_SPARSESET_HPP_
#define SOLAR_SPARSESET_HPP_

#include <vector>
#include <cstddef>
	
namespace sr
{
	template<typename Type = std::size_t>
	class SparseSet
	{
	public:
		SparseSet();
		SparseSet(std::size_t reserve);

		void insert(Type x);
		
		bool has(Type x);

		void reserve(std::size_t reserve);

		bool empty() noexcept;

		std::size_t size() const noexcept;

		std::size_t capacity() const noexcept;

	private:
		std::size_t m_size;
		std::size_t m_capacity;

		std::vector<Type> m_denseSet;
		std::vector<Type> m_sparseSet;
	};

	template<typename Type>
	inline SparseSet<Type>::SparseSet()
		:m_size(0), m_capacity(0)
	{
		static_assert(std::is_unsigned<T>::value, "SparseSet can only contain unsigned integers");
	}

	template<typename Type>
	inline SparseSet<Type>::SparseSet(std::size_t reserve)
		:m_size(0), m_capacity(0)
	{
		static_assert(std::is_unsigned<T>::value, "SparseSet can only contain unsigned integers");

		reserve(reserve);
	}

	template<typename Type>
	inline void SparseSet<Type>::insert(Type x)
	{
		m_denseSet.emplace_back(x);
		auto index = m_denseSet.size();
		m_sparseSet[x] = index;
	}

	template<typename Type>
	inline bool SparseSet<Type>::has(Type x)
	{
		bool success = false;

		if (m_denseSet[m_sparseSet[x]] == x)
		{
			success = true;
		}

		return success;
	}

	template<typename Type>
	inline void SparseSet<Type>::reserve(std::size_t reserve)
	{
		if (reserve > m_capacity)
		{
			m_denseSet.reserve(reserve);
			m_sparseSet.reserve(reserve);
			m_capacity = reserve;
		}
	}

	template<typename Type>
	inline bool SparseSet<Type>::empty() noexcept
	{
		return m_size == 0;
	}

	template<typename Type>
	inline std::size_t SparseSet<Type>::size() const noexcept
	{
		return m_size;
	}

	template<typename Type>
	inline std::size_t SparseSet<Type>::capacity() const noexcept
	{
		return m_capacity;
	}
}

#endif