///
/// Meta.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_META_HPP_
#define PROTOSTAR_META_HPP_

#include <tuple>
#include <vector>
#include <cstdint>
#include <algorithm>

///
/// Core namespace.
///
namespace protostar
{
	///
	/// \brief Used to determine if type or refernce should be returned based on condition.
	///
	/// Base template. Does nothing.
	///
	template<bool condition, class T = void>
	struct ReturnReferenceIfFalse
	{
	};

	///
	/// \brief Used to determine if type or reference should be returned based on condition.
	///
	/// If true, return plain type.
	///
	template<typename T>
	struct ReturnReferenceIfFalse<true, T>
	{
		typedef T type;
	};

	///
	/// \brief Used to determine if type or refernce should be returned based on condition.
	///
	/// If false, return reference.
	///
	template<typename T>
	struct ReturnReferenceIfFalse<false, T>
	{
		typedef T& type;
	};

	///
	/// \brief Do something to each item in tuple.
	///
	/// Different overload.
	/// Credits: https://stackoverflow.com/a/26902803.
	/// 
	template<class F, class...Ts, std::size_t...Is>
	inline void for_each_in_tuple(std::tuple<Ts...>& tuple, F func, std::index_sequence<Is...>) noexcept
	{
		// Expands a tuple.
		using expander = int[];
		(void)expander {
			0, ((void)func(std::get<Is>(tuple)), 0)...
		};
	}

	///
	/// \brief Do something to each item in tuple.
	///
	/// E.g. for_each_in_tuple(some, [](const auto &x) { std::cout << x << std::endl; });
	/// Credits: https://stackoverflow.com/a/26902803.
	/// 
	template<class F, class...Ts>
	inline void for_each_in_tuple(std::tuple<Ts...>& tuple, F func) noexcept
	{
		// Iterates over a tuple.
		for_each_in_tuple(tuple, func, std::make_index_sequence<sizeof...(Ts)>());
	}

	///
	/// \brief Iterates over a std::vector only keeps duplicates.
	///
	/// Credits: https://stackoverflow.com/a/58499766
	///		     https://stackoverflow.com/questions/58498959/keep-the-duplicated-values-only-vectors-c#comment103362833_58499766
	///
	template <typename T>
	void only_distinct_duplicates(std::vector<T>& v)
	{
		static_assert(std::is_arithmetic<T>::value);

		std::sort(v.begin(), v.end());

		auto output = v.begin();
		auto first = v.begin();
		const auto last = v.end();
		while (first != last) {
			const auto next{ std::find_if(first + 1, last, [&](const T& t) { return t != *first; }) };
			if (next - first > 1) {
				*output++ = *first;
			}
			first = next;
		}

		v.erase(output, last);
	}
}

#endif