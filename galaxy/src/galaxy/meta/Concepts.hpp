///
/// Concepts.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_META_CONCEPTS_HPP_
#define GALAXY_META_CONCEPTS_HPP_

#include <concepts>
#include <cstdint>
#include <string>
#include <type_traits>

namespace galaxy
{
	namespace meta
	{
		///
		/// Only class concept.
		///
		template<typename Type>
		concept is_class = std::is_class<Type>::value;

		///
		/// Concept to restrict templates to non pointers.
		///
		template<typename Type>
		concept not_pointer_or_ref = (!std::is_pointer<Type>::value) && (!std::is_reference<Type>::value);

		///
		/// Arithmetic concept.
		///
		template<typename Type>
		concept is_arithmetic = std::is_arithmetic<Type>::value;

		///
		/// Ensures a type is arithmetic or a std::string.
		///
		template<typename Type>
		concept standard_type = (std::is_arithmetic<Type>::value || std::is_same<std::string, Type>::value);

		///
		/// Makes sure a value is a flag for a std::bitset.
		///
		template<typename Type>
		concept is_flag = requires(Type type)
		{
			Type::value >= 0 && Type::value <= 7 && std::is_same<decltype(Type::value), unsigned short>::value;
		};
	} // namespace meta
} // namespace galaxy

#endif