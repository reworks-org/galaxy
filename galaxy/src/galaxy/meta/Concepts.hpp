///
/// Concepts.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_META_CONCEPTS_HPP_
#define GALAXY_META_CONCEPTS_HPP_

#include <concepts>
#include <string>
#include <type_traits>

namespace galaxy
{
	namespace meta
	{
		///
		/// \brief Only class concept.
		///
		/// Type must be a class or struct.
		///
		/// \tparam Type Type to test.
		///
		template<typename Type>
		concept is_class = std::is_class<Type>::value;

		///
		/// \brief Concept to restrict templates to not pointers and not references.
		///
		/// Type must not be a pointer or reference.
		///
		/// \tparam Type Type to test.
		///
		template<typename Type>
		concept not_memory = !std::is_pointer<Type>::value && !std::is_reference<Type>::value && std::is_object<Type>::value;

		///
		/// Arithmetic concept.
		///
		/// Type must be arithmetic. See: std::is_arithmetic.
		///
		/// \tparam Type Type to test.
		///
		template<typename Type>
		concept is_arithmetic = std::is_arithmetic<Type>::value;

		///
		/// \brief Ensures a type is arithmetic or a std::string.
		///
		/// Must be an arithmetic type or a std::string. See: std::is_arithmetic.
		///
		/// \tparam Type Type to test.
		///
		template<typename Type>
		concept standard_type = is_arithmetic<Type> || std::is_same<std::string, Type>::value;

		///
		/// \brief Tests for a type being a class, union, array, scalar or integral constant.
		///
		/// "Object" refers to any possibly cv-qualified type other than function, reference, or void types.
		///
		/// \tparam Type Type to test.
		///
		template<typename Type>
		concept is_object = std::is_object<Type>::value;

		///
		/// Makes sure a value is a valid value for a std::bitset.
		///
		/// Must be an unsigned short between 0 and 7. i.e. 8 values.
		///
		/// \tparam Type Type to test.
		///
		template<typename Type>
		concept is_bitset_flag = requires(Type type) { Type::value >= 0 && Type::value <= 7 && std::is_same<decltype(Type::value), unsigned short>::value; };

		///
		/// \brief Makes sure a type is a valid component.
		///
		/// I.e. Move assignable/constructible and a class with a json constructor.
		///
		/// \tparam Type Type to test.
		///
		template<typename Type>
		concept valid_component = std::is_move_assignable<Type>::value && std::is_move_constructible<Type>::value && std::is_class<Type>::value;

		///
		/// Makes sure a template specialization is an instance of a template class.
		///
		/// \tparam T Base type i.e. std::vector.
		/// \tparam U Specialization i.e. std::vector<int>
		///
		template<class T, template<class...> class U>
		inline constexpr bool is_instance_of_v = std::false_type {};
		template<template<class...> class U, class... Vs>
		inline constexpr bool is_instance_of_v<U<Vs...>, U> = std::true_type {};
	} // namespace meta
} // namespace galaxy

#endif
