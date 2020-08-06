///
/// Concepts.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_CONCEPTS_HPP_
#define PROTOSTAR_CONCEPTS_HPP_

#include <concepts>
#include <cstdint>
#include <string>
#include <type_traits>

///
/// Core namespace.
///
namespace pr
{
	///
	/// Ensures input is a positive float.
	///
	template<typename Type>
	concept positive_float = requires(Type val)
	{
		{std::is_same<Type, float>::value && (val > 0.0f)};
	};

	///
	/// Ensures input is a positive double.
	///
	template<typename Type>
	concept positive_double = requires(Type val)
	{
		{std::is_same<Type, double>::value && (val >= 0.0)};
	};

	///
	/// Ensures input is a positive short.
	///
	template<typename Type>
	concept positive_integer = requires(Type val)
	{
		{val >= 0};
	};

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
	/// Ensures that a pointer is not null.
	///
	template<typename Type>
	concept not_nullptr = requires(Type val)
	{
		{std::is_pointer<Type>::value::value && (val != nullptr)};
	};

	///
	/// Ensures that a pointer is a valid void pointer.
	///
	template<typename Type>
	concept valid_void_ptr = requires(Type val)
	{
		{std::is_same<Type, void*>::value::value && (val != nullptr)};
	};

	///
	/// Ensure a parameter is between 0 and 1.
	///
	template<typename Type>
	concept from_0_to_1 = requires(Type val)
	{
		{val >= 0 && val <= 1};
	};
} // namespace pr

#endif