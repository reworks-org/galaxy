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
	template<typename type>
	concept positive_float = requires(type val)
	{
		{std::is_same<type, float>::value && (val > 0)};
	};

	///
	/// Ensures input is a positive double.
	///
	template<typename type>
	concept positive_double = requires(type val)
	{
		{std::is_same<type, double>::value && (val > 0)};
	};

	///
	/// Ensures input is a positive short.
	///
	template<typename type>
	concept positive_short = requires(type val)
	{
		{std::is_same<type, short>::value && (val > 0)};
	};

	///
	/// Ensures input is a positive integer.
	///
	template<typename type>
	concept positive_int = requires(type val)
	{
		{std::is_same<type, int>::value && (val > 0)};
	};

	///
	/// Ensures input is a positive unsigned integer.
	///
	template<typename type>
	concept positive_uint = requires(type val)
	{
		{std::is_same<type, unsigned int>::value && (val > 0)};
	};

	///
	/// Ensures input is a positive long.
	///
	template<typename type>
	concept positive_long = requires(type val)
	{
		{std::is_same<type, long>::value && (val > 0)};
	};

	///
	/// Ensures input is a positive std::size_t.
	///
	template<typename type>
	concept positive_size_t = requires(type val)
	{
		{std::is_same<type, std::size_t>::value && (val > 0)};
	};

	///
	/// Ensures input is a positive std::uint8_t.
	///
	template<typename type>
	concept positive_uint8_t = requires(type val)
	{
		{std::is_same<type, std::uint8_t>::value && (val > 0)};
	};

	///
	/// Only class concept.
	///
	template<typename type>
	concept is_class = std::is_class<type>::value;

	///
	/// Concept to restrict templates to non pointers.
	///
	template<typename type>
	concept not_pointer_or_ref = (!std::is_pointer<type>::value) && (!std::is_reference<type>::value);

	///
	/// Arithmetic concept.
	///
	template<typename type>
	concept is_arithmetic = std::is_arithmetic<type>::value;

	///
	/// Ensures that a pointer is not null.
	///
	template<typename type>
	concept not_nullptr = requires(type val)
	{
		{std::is_pointer<type>::value::value && (val != nullptr)};
	};

	///
	/// Ensures that a pointer is a valid void pointer.
	///
	template<typename type>
	concept not_nullptr_void = requires(type val)
	{
		{std::is_same<type, void*>::value::value && (val != nullptr)};
	};

	///
	/// Ensure a parameter is between 0 and 1.
	///
	template<typename type>
	concept from_0_to_1 = requires(type val)
	{
		{val <= 1 && val >= 0};
	};
} // namespace pr

#endif