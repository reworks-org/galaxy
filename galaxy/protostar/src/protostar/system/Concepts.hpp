///
/// Concepts.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_CONCEPTS_HPP_
#define PROTOSTAR_CONCEPTS_HPP_

#include <cstdint>
#include <concepts>
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
		{std::is_same<Type, float>::value && (val > 0)};
	};

	///
	/// Ensures input is a positive double.
	///
	template<typename Type>
	concept positive_double = requires(Type val)
	{
		{std::is_same<Type, double>::value && (val > 0)};
	};

	///
	/// Ensures input is a positive short.
	///
	template<typename Type>
	concept positive_short = requires(Type val)
	{
		{std::is_same<Type, short>::value && (val > 0)};
	};

	///
	/// Ensures input is a positive integer.
	///
	template<typename Type>
	concept positive_int = requires(Type val)
	{
		{std::is_same<Type, int>::value && (val > 0)};
	};

	///
	/// Ensures input is a positive unsigned integer.
	///
	template<typename Type>
	concept positive_uint = requires(Type val)
	{
		{std::is_same<Type, unsigned int>::value && (val > 0)};
	};

	///
	/// Ensures input is a positive long.
	///
	template<typename Type>
	concept positive_long = requires(Type val)
	{
		{std::is_same<Type, long>::value && (val > 0)};
	};

	///
	/// Ensures input is a positive std::size_t.
	///
	template<typename Type>
	concept positive_size_t = requires(Type val)
	{
		{std::is_same<Type, std::size_t>::value && (val > 0)};
	};

	///
	/// Ensures input is a positive std::uint8_t.
	///
	template<typename Type>
	concept positive_uint8_t = requires(Type val)
	{
		{std::is_same<Type, std::uint8_t>::value && (val > 0)};
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
	concept not_nullptr_void = requires(Type val)
	{
		{std::is_same<Type, void*>::value::value && (val != nullptr)};
	};

	///
	/// Ensure a parameter is between 0 and 1.
	///
	template<typename Type>
	concept from_0_to_1 = requires(Type val)
	{
		{val <= 1 && val >= 0};
	};
} // namespace pr

#endif