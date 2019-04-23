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

		void insert(Type x);
		bool has(Type x);

		void reserve();
		bool empty();
		std::size_t size();
		std::size_t capacity();

	private:
		std::size_t m_size;
		std::size_t m_capacity;

		std::vector<Type> m_denseSet;
		std::vector<Type> m_sparseSet;
	};

	template<typename Type>
	inline SparseSet<Type>::SparseSet()
	{
		static_assert(std::is_unsigned<T>::value, "SparseSet can only contain unsigned integers");
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
}

#endif